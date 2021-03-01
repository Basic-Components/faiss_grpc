from google.protobuf.json_format import MessageToDict
from .cli import client
from .faiss_rpc_pb2 import Query, BatchQuery, Vec


def cli_exp(url: str = "localhost:5000") -> None:
    # req-res
    with client.initialize_from_url(url) as conn:
        res = conn.search(Query(
            target_index="example",
            k=4,
            query_vec=Vec(elements=[])
            ))
        print(MessageToDict(res))
        # stream-stream

        # for bid in range(5):
        #     q = BatchQuery(
        #         batch_id = str(bid)
        #         target_index="example",
        #         k=4,
        #         query_vecs=[
        #             Vec(elements=[]), ]
        #     )
        # res_stream = conn.StreamrangeSquare((BatchQuery(Message=float(i)) for i in range(4)))
        # for res in res_stream:
        #     print(MessageToDict(res))
