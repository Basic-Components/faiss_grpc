docker buildx build --target=faiss-grpc-cpu --platform linux/amd64 -t hsz1273327/faiss-grpc-cpu:amd64-0.0.0 .
# docker buildx build --target=faiss-grpc-cpu --platform linux/arm64 -t hsz1273327/faiss-grpc-cpu:arm64-0.0.0 .

docker push -a  hsz1273327/faiss-grpc-cpu