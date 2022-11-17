# faiss_grpc

这个项目的目的是使用grpc封装faiss来线上查找的相似度top k.

**注意**: 本项目不再维护,如果需要类似功能,请使用[milvus](https://github.com/milvus-io/milvus)

## 特性

1. 监听文件系统变化以刷新index
2. 支持请求响应模式和双向流模式两种接口

## 使用方法

已经使用docker打包了如下两个镜像列表:

+ `hsz1273327/faiss-grpc-cpu`支持amd64平台,使用cpu进行计算,支持的tag为`amd64-0.2.0`
<!-- + `hsz1273327/faiss-grpc-gpu`支持amd64和arm64平台,使用cpu进行计算 -->

执行`docker run -v localindexdirpath:/indexes hsz1273327/faiss-grpc-cpu`即可,`localindexdirpat`指代你的索引文件所在文件夹路径

### 配置参数

本项目可以通过配置文件,环境变量,命令行参数进行设置,其覆盖顺序为命令行>环境变量>配置文件>默认.配置文件只读取执行目录下的`config.json`文件(容器中为`/config.json`)

#### 服务名`app_name`

+ 命令行:`--app_name`
+ 环境变量: `FAISS_GRPC_APP_NAME`
+ 配置文件字段:`app_name`
+ 默认值: `"faiss_grpc"`

#### 服务版本`app_version`

+ 命令行:`--app_version`
+ 环境变量: `FAISS_GRPC_APP_VERSION`
+ 配置文件字段:`app_version`
+ 默认值: `"0.0.2"`

#### 服务启动地址`address`

+ 命令行:`--address`和`-a`
+ 环境变量: `FAISS_GRPC_ADDRESS`
+ 配置文件字段:`address`
+ 默认值: `"0.0.0.0:5000"`

#### log等级`log_level`

+ 命令行:`--log_level`
+ 环境变量: `FAISS_GRPC_LOG_LEVEL`
+ 配置文件字段:`log_level`
+ 默认值: `"debug"`
+ 枚举:`"trace", "debug", "info", "warn", "err", "critical"`

#### 服务的证书`server_cert_path`

+ 命令行:`--server_cert_path`
+ 环境变量: `FAISS_GRPC_SERVER_CERT_PATH`
+ 配置文件字段:`server_cert_path`
+ 默认值: `""`

#### 服务证书的私钥`server_key_path`

+ 命令行:`--server_key_path`
+ 环境变量: `FAISS_GRPC_SERVER_KEY_PATH`
+ 配置文件字段:`server_key_path`
+ 默认值: `""`

#### 服务使用的根证书`root_cert_path`

+ 命令行:`--root_cert_path`
+ 环境变量: `FAISS_GRPC_ROOT_CERT_PATH`
+ 配置文件字段:`--root_cert_path`
+ 默认值: `""`

#### 客户端请求时的验证模式`client_certificate_request`

+ 命令行:`--client_certificate_request`
+ 环境变量: `FAISS_GRPC_CLIENT_CERT_REQUEST`
+ 配置文件字段:`client_certificate_request`
+ 默认值: `""`
+ 枚举:`"","not_request","not_verify","request_and_verify","enforce_request","enforce_request_and_verify"`

#### 最大接收消息大小`max_rec_msg_size`

+ 命令行:`--max_rec_msg_size`
+ 环境变量: `FAISS_GRPC_MAX_REC_MSG_SIZE`
+ 配置文件字段:`max_rec_msg_size`
+ 默认值: `1024 * 1024`

#### 最大发送消息大小`max_send_msg_size`

+ 命令行:`--max_send_msg_size`
+ 环境变量: `FAISS_GRPC_MAX_SEND_MSG_SIZE`
+ 配置文件字段:`max_send_msg_size`
+ 默认值: `1024 * 1024`

#### 流中的最大并发数`max_concurrent_streams`

+ 命令行:`--max_concurrent_streams`
+ 环境变量: `FAISS_GRPC_MAX_CONCURRENT_STREAMS`
+ 配置文件字段:`max_concurrent_streams`
+ 默认值: `100`

#### 连接的最大空闲时长`max_connection_idle`

+ 命令行:`--max_connection_idle`
+ 环境变量: `FAISS_GRPC_MAX_CONNECTION_IDLE`
+ 配置文件字段:`max_connection_idle`
+ 默认值: `0`

#### 连接的最大存活时间`max_connection_age`

+ 命令行:`--max_connection_age`
+ 环境变量: 
+ 配置文件字段:`max_connection_age`
+ 默认值: `0`

#### 杀死连接的最大等待时间`max_connection_age_grace`

+ 命令行:`--max_connection_age_grace`
+ 环境变量:`FAISS_GRPC_MAX_CONNECTION_AGE_GRACE`
+ 配置文件字段:`max_connection_age_grace`
+ 默认值: `0`

#### 连接的保持存活时长`keepalive_time`

+ 命令行:`--keepalive_time`
+ 环境变量: `FAISS_GRPC_KEEPALIVE_TIME`
+ 配置文件字段:`keepalive_time`
+ 默认值: `0`

#### 连接的保持存活时长`keepalive_timeout`

+ 命令行:`--keepalive_timeout`
+ 环境变量: `FAISS_GRPC_KEEPALIVE_TIMEOUT`
+ 配置文件字段:`keepalive_timeout`
+ 默认值: `0`

#### 是否没有活动流就不允许ping操作`not_keepalive_enforement_permit_without_stream`

+ 命令行:`--not_keepalive_enforement_permit_without_stream`
+ 环境变量: `FAISS_GRPC_KEEPALIVE_ENFORCEMENT_PERMIT_WITHOUT_STREAM`
+ 配置文件字段:`not_keepalive_enforement_permit_without_stream`
+ 默认值: `false`

#### 数据压缩格式`compression`

+ 命令行:`--compression`
+ 环境变量: `FAISS_GRPC_COMPRESSION`
+ 配置文件字段:`compression`
+ 默认值: `""`
+ 枚举值
    + `""`不进行压缩
    + `"deflate", "gzip"`使用指定格式压缩

#### faiss的索引所在的文件夹`index_dirs`

+ 命令行:`--index_dirs`
+ 环境变量:
+ 配置文件字段:`index_dirs`
+ 默认值: `{"./indexes"}`

## 自己打包镜像

`Dockerfile`已经提供,可以使用`docker build`自己打镜像

## 测试功能

测试使用我自己的脚手架`pmfp`,执行命令记录在`test.sh`中.请求的payload保存在`testquery`文件夹下.测试时记得先本地5000端口起起来服务(如果有docker环境可以直接执行`docker-compose up`).

## 关于编译的说明

本项目使用了几个本人自己封装的包,这几个包由于conan仓库的原因目前只存在自家仓库里.