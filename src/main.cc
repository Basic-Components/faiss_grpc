# include "faiss_grpc_serv/faiss_grpc_serv.h"

int main(int argc, char** argv) {
    faiss_grpc_serv::RootNode root;
    root.set_name("faiss_grpc");
    root.set_schema(faiss_grpc_serv::SCHEMA);
    root.exec(argc,argv);
    return 0;
}