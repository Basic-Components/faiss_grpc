FROM dev.hszofficial.site:9443/library/vscode_cxx_env:alpine3.13-gcc10-conan1.39.0-vcpkgmaster
RUN apk add openblas-static lapack-dev