g++ -D_GLIBCXX_USE_CXX11_ABI=0 test.cc -o demo_imi_flat  ../faiss/libfaiss.a /usr/lib/gcc/x86_64-linux-gnu/8/libgomp.so /usr/lib/x86_64-linux-gnu/libpthread.so /usr/lib/x86_64-linux-gnu/libopenblas.so -lpthread -lm -ldl 


g++ -std=c++17 -Llibs -Iincludes -I/usr/include libs/libfaiss.a *.cc `pkg-config --cflags protobuf grpc` -lstdc++fs -lgomp -lpthread -lopenblas -pthread -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o faiss_rpc_server

g++ -std=c++17 -Iincludes test/jsonload.cc -o test/jsonload