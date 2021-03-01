#cython: language_level=3
import cython
import multiprocessing
from concurrent import futures
from functools import partial
from typing import Any, Dict, List
import grpc
from pyloggerhelper import log
from .faiss_rpc_pb2_grpc import FaissRpcServicer
from .faiss_rpc_pb2 import Response,BatchResponse,ResponseStatus,TopK
from .index import FAISS_INDEX_MAP,FAISS_INDEX_MAP_LOCK
from .faiss_search import faiss_search


def faiss_search(vecs: List[List[float]], k:int, index:Any) -> List[List[int]]:
    _, ind = index.search(np.array(vecs, dtype="float32"), k)
    return ind.tolist()


class Handdler(FaissRpcServicer):
    CacheIndexPathTemplate = "{root_dir}/{target_platform}/model/recall/ann_search_rpc/{item_type}-{algo_id}-{gender}.index"

    def __init__(self, config: Dict[str, Any]):
        super().__init__()
        self.config = config
        log.info("获取服务配置", config=self.config)
        workers = config.get("search_workers", 4)
        if workers <= 0:
            workers = multiprocessing.cpu_count()
        self.aioexcutor = futures.ThreadPoolExecutor(max_workers=workers)


    async def _search(self, query_vecs, k: int, target_index: str) -> List[List[int]]:
        vecs = [i.elements for i in query_vecs]
        with FAISS_INDEX_MAP_LOCK.gen_rlock():
            index = FAISS_INDEX_MAP.get(target_index)
        if index:
            fn = partial(faiss_search,vecs=vecs,k=k,index=index)
            res = await loop.run_in_executor(self.aioexcutor, fn)
            return res
        else:
            raise AttributeError(f"index {index_key} not found")

    

    async def search(self, request: Any, context: grpc.ServicerContext) -> Any:
        if request.k <= 0 or request.k > 30:
            return Response(status=ResponseStatus(status=ResponseStatus.Stat.FAILED, message=f"query args k must in (0,30),find {request.k}"))
        if not request.target_index:
            return Response(status=ResponseStatus(status=ResponseStatus.Stat.FAILED, message="query args must have target_index"))
        if not request.query_vec:
            return Response(status=ResponseStatus(status=ResponseStatus.Stat.FAILED, message="query args must have  query_vec"))
        try:
            res = await self._search(
                vecs=vecs,
                k=request.k,
                query_vecs = [request.query_vec])
            
        except Exception as e:
            return Response(status=ResponseStatus(status=ResponseStatus.Stat.FAILED, message=str(e)))
        else:
            log.info("get result", res=res)
            return Response(status=ResponseStatus(status=ResponseStatus.Stat.SUCCEED), result=[TopK(rank=i) for i in res])

    async def batch_search(self, request_iterator: Any, context: grpc.ServicerContext) -> Any:
        for request in request_iterator:
            if not request.batch_id:
                yield BatchResponse(status=ResponseStatus(status=ResponseStatus.Stat.FAILED, message="query args must have batch_id"))
                continue
            if request.k <= 0 or request.k > 30:
                yield BatchResponse(status=ResponseStatus(status=ResponseStatus.Stat.FAILED, message=f"query {request.batch_id} args k must in (0,30),find {request.k}"))
                continue
            if not request.target_index:
                yield Response(status=ResponseStatus(status=ResponseStatus.Stat.FAILED, message=f"query  {request.batch_id}  args must have target_index"))
                continue
            if not request.query_vecs:
                yield BatchResponse(status=ResponseStatus(status=ResponseStatus.Stat.FAILED, message=f"query {request.batch_id} must have query_vecs"))
                continue
            try:
                res = await self._search(
                vecs=vecs,
                k=request.k,
                query_vecs = request.query_vecs)
            except Exception as e:
                yield Response(status=ResponseStatus(status=ResponseStatus.Stat.FAILED, message=f"query {request.batch_id} get error {str(e)}"))
            else:
                log.info("get result", res=res)
                yield BatchResponse(status=ResponseStatus(status=ResponseStatus.Stat.SUCCEED), batch_id=request.batch_id,result=[TopK(rank=i) for i in res])
