# 编译faiss
#FROM --platform=$TARGETPLATFORM debian:buster-slim as build-lib-cpu
FROM debian:buster-slim as build-lib-cpu
ARG FAISS_VERSION=1.7.0
WORKDIR /code
# 安装依赖
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN apt update -y 
## 编译环境依赖
RUN apt install -y --no-install-recommends build-essential autoconf libtool pkg-config upx-ucl
## faiss 编译和执行依赖
RUN apt install -y --no-install-recommends libopenblas-dev
## grpc服务执行依赖
RUN apt install -y --no-install-recommends libprotobuf-dev libgrpc++-dev libssl-dev libfswatch-dev
## grpc 转译依赖
RUN apt install -y --no-install-recommends protobuf-compiler protobuf-compiler-grpc
RUN rm -rf /var/lib/apt/lists/*
## cmake安装(buster版本过低) 
RUN mkdir tools
COPY cmake-${BUILDARCH}.sh /code/cmake-${BUILDARCH}.sh
RUN ./cmake-${BUILDARCH}.sh --skip-license --prefix=tools
# 其他第三方依赖根据网络情况选择安装方式,需要修改CMakeLists.txt
COPY src /code/src
COPY thirdpart /code/thirdpart
COPY CMakeLists.txt /code/CMakeLists.txt
RUN ./tools/bin/cmake .
RUN make
RUN mkdir upxbin
RUN upx --best --lzma -o /code/upxbin/faiss_grpc /code/bin/faiss_grpc 

# 部署
#FROM --platform=$TARGETPLATFORM debian:buster-slim as bin
FROM debian:buster-slim as bin
# 安装依赖
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN apt update -y 
## faiss 编译和执行依赖
RUN apt install -y --no-install-recommends libopenblas-dev
## grpc服务执行依赖
RUN apt install -y --no-install-recommends libprotobuf-dev libgrpc++-dev libssl-dev libfswatch-dev
RUN rm -rf /var/lib/apt/lists/*
COPY --from=build-lib-cpu /code/upxbin/faiss_grpc .
# 赋予执行权限
RUN chmod +x faiss_grpc
ENTRYPOINT  ["./faiss_grpc"]
