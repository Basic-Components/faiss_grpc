ppm grpc query -p -s faiss_grpc.FAISS_GRPC -m get_index_metadata --payload=testquery/metaquery.json localhost:5000 # 测试meta
ppm grpc query -p -s faiss_grpc.FAISS_GRPC -m get_index_list --payload=testquery/listquery.json localhost:5000 # 测试list
ppm grpc query -p -s faiss_grpc.FAISS_GRPC -m search --payload=testquery/searchquery.json localhost:5000 # 测试search
ppm grpc query -p -s faiss_grpc.FAISS_GRPC -m reload_index --payload=testquery/reloadquery.json localhost:5000 # 测试reload