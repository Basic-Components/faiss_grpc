# 编译faiss的步骤
FROM debian:buster-slim as faiss_builder
ARG FAISS_VERSION=1.7.0
WORKDIR /code
# 安装grpc
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN apt update -y && \
    apt install -y --no-install-recommends build-essential autoconf libtool pkg-config && \
    apt install -y --no-install-recommends libopenblas-dev libblis-dev libblas-dev libssl-dev && \
    rm -rf /var/lib/apt/lists/*

RUN ls /usr/local/bin
# 构造cmake
ADD https://github.com/Kitware/CMake/releases/download/v3.19.6/cmake-3.19.6.tar.gz /code
RUN tar -zxvf cmake-3.19.6.tar.gz
WORKDIR /code/cmake-3.19.6
RUN ./bootstrap
RUN make
RUN make install
RUN ln -s /usr/local/bin/cmake /usr/bin/cmake
RUN rm -f cmake-3.19.6.tar.gz
RUN rm -rf cmake-3.19.6
# 编译faiss
WORKDIR /code
ADD https://github.com/facebookresearch/faiss/archive/v1.7.0.tar.gz /code
RUN tar -zxvf faiss-${FAISS_VERSION}.tar.gz
WORKDIR /code/faiss-${FAISS_VERSION}
RUN cmake -DFAISS_ENABLE_GPU=OFF -DFAISS_ENABLE_PYTHON=OFF -DBUILD_TESTING=OFF -B build .
RUN make -C build -j faiss
RUN cp faiss-${FAISS_VERSION}/build/faiss/libfaiss.a libs/libfaiss.a 
# COPY pbschema /code/pbschema
# COPY serv.cc /code/serv.cc
# COPY makefile /code/makefile
# RUN make
RUN rm -f faiss-${FAISS_VERSION}.tar.gz
RUN rm -rf faiss-${FAISS_VERSION}

# # 构造grpc服务的步骤
# FROM --platform=$TARGETPLATFORM debian:buster-slim as grpc_builder
# WORKDIR /code
# # 安装grpc
# RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
# RUN apt update -y && \
#     apt install -y --no-install-recommends build-essential autoconf libtool pkg-config && \
#     apt install -y --no-install-recommends libopenblas-dev libblis-dev libblas-dev libssl-dev libfswatch-dev && \
#     apt install -y --no-install-recommends libprotobuf-dev libgrpc++-dev protobuf-compiler protobuf-compiler-grpc libspdlog-dev && \
#     rm -rf /var/lib/apt/lists/*



# COPY pbschema /code/pbschema
# COPY serv.cc /code/serv.cc
# COPY makefile /code/makefile
# RUN make