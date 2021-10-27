# 构造可执行文件
FROM --platform=$TARGETPLATFORM dev.hszofficial.site:9443/library/buildx_cxx_env:alpine3.13-gcc10-conan1.41.0-protoc3.17.1-grpc1.39.1 as build_bin
RUN apk add --no-cache openblas-static lapack-dev && rm -rf /var/cache/apk/* 
WORKDIR /code
COPY CMakeLists.txt CMakeLists.txt
COPY conanfile.txt conanfile.txt
# 添加源文件
COPY src/ src/
WORKDIR /code/build
# 编译
RUN conan install .. --build=missing
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
RUN cmake --build .

# 编译获得grpc-health-probe
FROM --platform=$TARGETPLATFORM golang:1.17.0-buster as build_grpc-health-probe
ENV GO111MODULE=on
ENV GOPROXY=https://goproxy.io
# 停用cgo
ENV CGO_ENABLED=0
# 安装grpc-health-probe
RUN go get github.com/grpc-ecosystem/grpc-health-probe

# 使用压缩过的可执行文件构造镜像
FROM --platform=$TARGETPLATFORM alpine:3.14 as build_img
# 健康检测
COPY --from=build_grpc-health-probe /go/bin/grpc-health-probe /grpc-health-probe
# 应用
COPY --from=build_bin /code/build/bin/faiss_grpc /faiss_grpc 
EXPOSE 5000
RUN chmod +x /faiss_grpc
RUN chmod +x /grpc-health-probe
HEALTHCHECK --interval=30s --timeout=30s --start-period=5s --retries=3 CMD [ "/grpc-health-probe","-addr=:5000" ]
ENTRYPOINT [ "/faiss_grpc"]