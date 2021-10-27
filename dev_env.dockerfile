FROM dev.hszofficial.site:9443/library/vscode_cxx_env:alpine3.13-gcc10-conan1.41.0-protobuf3.17.1-grpc1.39.1
RUN apk add openblas-static lapack-dev