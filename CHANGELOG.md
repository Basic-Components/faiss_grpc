# 0.0.3

## 新特性

1. 修改监听逻辑,现在和tf-serving一致,`index名->index版本->xxx.index`
2. 移出`--index-dir`,改为使用参数`--index-config-file`和`--index-config-poll_wait_seconds`指定配置文件来监听索引变化.逻辑参考tf-serving

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
