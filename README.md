# faiss_grpc

+ author: hsz
+ author-email: hsz1273327@gmail.com

keywords: faiss, grpc

## 特性

+ 提供单条请求接口`search`
+ 提供批处理流式接口`batch_search`
+ 指定配置项`index_dirs`(默认执行目录下的`indexes`文件夹)后自动加载后缀为`.index`的faiss的index文件.


## 依赖

+ linux系统(只在debian:buster中编译过)
+ [faiss 1.7.0](https://github.com/facebookresearch/faiss) (下载后编译放到libs文件夹)
+ [gabime/spdlog](https://github.com/gabime/spdlog) (下载后编译安装)
+ [nlohmann/json](https://github.com/nlohmann/json#arbitrary-types-conversions) (下载后将头文件放至includes文件夹)
+ [muellan/clipp](https://github.com/muellan/clipp) (下载后将头文件放至includes文件夹)

## 编译

根目录下执行`make`即可,会编译为可执行文件`faiss_rpc_server`

## 使用

可以编译好后使用,也可以通过docker使用.

### 配置项

### 读取配置

+ 默认从执行目录下尝试读取`config.json`作为配置文件
+

