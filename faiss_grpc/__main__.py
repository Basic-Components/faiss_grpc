import sys
from schema_entry import EntryPoint
from faiss_rpc import Serv
from faiss_rpc.cli_example import cli_exp
from make_index import make_index


def main() -> None:
    root = EntryPoint(name="faiss_grpc", description="faiss的grpc服务工具")
    root.regist_sub(Serv)
    root.regist_sub(
        EntryPoint,
        name="cli",
        schema={
            "$schema": "http://json-schema.org/draft-07/schema#",
            "type": "object",
            "properties": {
                    "url": {
                        "type": "string",
                        "default": "localhost:5000"
                    }
            }
        },
        main=cli_exp)

    root.regist_sub(
        EntryPoint,
        name="make_index",
        schema={
            "$schema": "http://json-schema.org/draft-07/schema#",
            "type": "object",
            "properties": {
                    "index_dir": {
                        "type": "string",
                        "default": "/localsource/test"
                    }
            }
        },
        main=make_index)
    
    root(sys.argv[1:])