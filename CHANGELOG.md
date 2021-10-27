# 0.0.2

## 新特性

1. 增加了批处理接口`batch_search`
2. 增加了`get_index_list`,`get_index_metadata`接口用于获取服务状态
3. 增加了`reload_index`接口用于手动重载index

## 依赖更新

+ `grpc -> 1.39.1`
+ `faiss -> 1.7.1`

## 修改实现

1. 改用conan维护依赖
2. 改用apline作为基镜像编译纯静态可执行文件

# 0.0.1

项目创建