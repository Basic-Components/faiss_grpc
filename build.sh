# 		   /usr/lib/gcc/x86_64-linux-gnu/8/libgomp.so \
# 		   /usr/lib/x86_64-linux-gnu/libpthread.so \
# 		   /usr/lib/x86_64-linux-gnu/libopenblas.so \

g++ -Iincludes -std=c++17 faiss_index_manager.cc libs/libfaiss.a -o test_m -lstdc++fs -lgomp -lpthread -lopenblas