# 编译faiss
FROM --platform=${TARGETPLATFORM} debian:buster-slim as build-faiss
#FROM debian:buster-slim as build-cpu
#ARG FAISS_VERSION=1.7.0
ARG TARGETARCH
WORKDIR /code
# 安装依赖
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN apt update -y 
## 编译环境依赖
RUN apt install -y --no-install-recommends build-essential autoconf libtool pkg-config upx-ucl libssl-dev git curl ca-certificates
## faiss 编译和执行依赖
RUN apt install -y --no-install-recommends libopenblas-dev
## grpc服务执行依赖
RUN apt install -y --no-install-recommends libprotobuf-dev libgrpc++-dev libfswatch-dev
## grpc 转译依赖
RUN apt install -y --no-install-recommends protobuf-compiler protobuf-compiler-grpc
RUN rm -rf /var/lib/apt/lists/*
## cmake安装(buster版本过低) 
RUN mkdir tools
COPY cmake-${TARGETARCH}.sh /code/cmake-${TARGETARCH}.sh
RUN ./cmake-${TARGETARCH}.sh --skip-license --prefix=tools

# 其他第三方依赖根据网络情况选择安装方式,需要修改CMakeLists.txt
COPY cmake/ /code/cmake/
# COPY inc /code/inc
COPY pbschema/ /code/pbschema/
COPY src/ /code/src/
COPY thirdpart/ /code/thirdpart/
COPY CMakeLists.txt /code/CMakeLists.txt
# 安装cmake无法管理的依赖
# 安装faiss
WORKDIR /code/thirdpart/faiss-1.7.0
RUN /code/tools/bin/cmake -DFAISS_ENABLE_GPU=OFF -DFAISS_ENABLE_PYTHON=OFF -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release -B build .
RUN make -C build -j4 faiss
RUN make -C build install
WORKDIR /code

FROM build-faiss as build-cpu
WORKDIR /code
# 编译项目
RUN ./tools/bin/cmake .
RUN make VERBOSE=1
# 压缩可执行文件
RUN mkdir upxbin
RUN upx --best --lzma -o /code/upxbin/faiss_grpc /code/bin/faiss_grpc 

# 部署
FROM --platform=${TARGETPLATFORM} debian:buster-slim as faiss-grpc-cpu
#FROM debian:buster-slim as faiss-grpc-cpu
# 安装依赖
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN apt update -y 
## faiss 编译和执行依赖
RUN apt install -y --no-install-recommends libopenblas-dev libgomp1
## grpc服务执行依赖
RUN apt install -y --no-install-recommends libprotobuf-dev libgrpc++-dev libssl-dev libfswatch-dev
RUN rm -rf /var/lib/apt/lists/*
COPY --from=build-cpu /code/upxbin/faiss_grpc .
# 赋予执行权限
RUN chmod +x faiss_grpc
ENTRYPOINT  ["./faiss_grpc"]
