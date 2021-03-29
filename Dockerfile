# 编译faiss
FROM --platform=${TARGETPLATFORM} debian:buster-slim as build-cpu
#FROM debian:buster-slim as build-cpu
#ARG FAISS_VERSION=1.7.0
ARG BUILDARCH 
WORKDIR /code
# 安装依赖
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN apt update -y 
## 编译环境依赖
RUN apt install -y --no-install-recommends build-essential autoconf libtool pkg-config upx-ucl libssl-dev git curl
## faiss 编译和执行依赖
RUN apt install -y --no-install-recommends libopenblas-dev
## grpc服务执行依赖
RUN apt install -y --no-install-recommends libprotobuf-dev libgrpc++-dev libfswatch-dev
## grpc 转译依赖
RUN apt install -y --no-install-recommends protobuf-compiler protobuf-compiler-grpc
RUN rm -rf /var/lib/apt/lists/*
## cmake安装(buster版本过低) 
RUN mkdir tools
COPY cmake-${BUILDARCH}.sh /code/cmake-${BUILDARCH}.sh
RUN ./cmake-${BUILDARCH}.sh --skip-license --prefix=tools
# RUN git config --global http.sslverify false
# RUN git config --global http.sslBackend "openssl"
# ADD https://github.com/facebookresearch/faiss/archive/refs/tags/v1.7.0.tar.gz /code/
# ADD https://github.com/nlohmann/json/archive/v3.9.1.tar.gz /code/
# ADD https://github.com/gabime/spdlog/archive/v1.8.2.tar.gz /code/
# ADD https://github.com/p-ranav/argparse/archive/refs/tags/v2.0.tar.gz /code/

# RUN tar -zxvf faiss-1.7.0.tar.gz -C /code/thirdpart

# 其他第三方依赖根据网络情况选择安装方式,需要修改CMakeLists.txt
COPY cmake/ /code/cmake/
#COPY includes/faiss /usr/include/faiss
COPY pbschema/ /code/pbschema/
COPY src/ /code/src/
#COPY thirdpart/ /code/thirdpart/
COPY CMakeLists.txt /code/CMakeLists.txt
#RUN mkdir lib
RUN ./tools/bin/cmake -DFAISS_ENABLE_GPU=OFF -DFAISS_ENABLE_PYTHON=OFF -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release -DUSE_SYSTEM_CURL=ON .
RUN make
RUN mkdir upxbin
RUN upx --best --lzma -o /code/upxbin/faiss_grpc /code/bin/faiss_grpc 

# 部署
FROM --platform=${TARGETPLATFORM} debian:buster-slim as faiss-grpc-cpu
#FROM debian:buster-slim as faiss-grpc-cpu
# 安装依赖
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN apt update -y 
## faiss 编译和执行依赖
RUN apt install -y --no-install-recommends libopenblas-dev
## grpc服务执行依赖
RUN apt install -y --no-install-recommends libprotobuf-dev libgrpc++-dev libssl-dev libfswatch-dev
RUN rm -rf /var/lib/apt/lists/*
COPY --from=build-cpu /code/upxbin/faiss_grpc .
# 赋予执行权限
RUN chmod +x faiss_grpc
ENTRYPOINT  ["./faiss_grpc"]
