import sys
from faiss_rpc import Serv

server = Serv()
server(sys.argv[1:])
