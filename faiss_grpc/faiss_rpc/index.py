from pathlib import Path
from hashlib import md5
from typing import List
import faiss
from pyloggerhelper import log
from readerwriterlock import rwlock
from watchdog.events import PatternMatchingEventHandler

FAISS_INDEX_MAP = {}

FAISS_INDEX_MAP_LOCK = rwlock.RWLockFairD()


def load_indexes(index_dirs: List[str]) -> None:
    log.info("load_indexes start")
    for root in index_dirs:
        rootp = Path(root)
        if not rootp.is_dir():
            raise AttributeError(f"root {root} not dir")
        for sub in rootp.iterdir():
            if sub.is_file() and sub.suffix == ".index":
                index_name = sub.name.replace(".index", "")
                index = faiss.read_index(str(sub))
                log.info("get index", index_name=index_name)
                with FAISS_INDEX_MAP_LOCK.gen_wlock():
                    FAISS_INDEX_MAP[index_name] = index
                    log.info("load index ok", index_name=index_name)


class UpdateIndexes(PatternMatchingEventHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.latestmd5 = {}

    def on_closed(self, event):
        """Called when a file opened for writing is closed.
        :param event:
            Event representing file closing.
        :type event:
            :class:`FileClosedEvent`
        """
        try:
            srcp = Path(event.src_path)
            index_name = srcp.name.replace(".index", "")
            with open(event.src_path, "rb") as f:
                content = f.read()
                if len(content) < 100:
                    log.info("文件字节数量小于100,未更新", index_name=index_name)
                    return
                m = md5()
                m.update(content)
                newmd5 = m.digest()
                latestmd5 = self.latestmd5.get(event.src_path)
                if latestmd5:
                    if newmd5 == latestmd5:
                        log.info("两次内容相同,未更新", index_name=index_name)
                        return
                self.latestmd5[event.src_path] = newmd5
                index = faiss.read_index(event.src_path)
                
                with FAISS_INDEX_MAP_LOCK.gen_wlock():
                    FAISS_INDEX_MAP[index_name] = index
                log.info("load index ok", index_name=index_name)
        except Exception as e:
            log.error("UpdateIndexes get error", 
                exc_info=True,
                stack_info=True)
