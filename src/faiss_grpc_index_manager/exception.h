#pragma once
#include <string>
#include <exception>

namespace faiss_grpc_index_manager{
    // 定义异常
    //DNotEqualException 维度不一致异常
    class DNotEqualException : public std::exception{
        public:
        DNotEqualException() : message("d not equal") {};
        DNotEqualException(std::string str) : message("d not equal," + str) {};
        ~DNotEqualException() throw(){};
        virtual const char *what() const throw(){
            return message.c_str();
        };
        private:
        std::string message;
    };
    //IndexNotLoadException index未加载异常
    class IndexNotLoadException : public std::exception{
        public:
        IndexNotLoadException() : message("index not load yet") {};
        IndexNotLoadException(std::string str) : message("index not load yet: " + str) {};
        ~IndexNotLoadException() throw(){};
        virtual const char *what() const throw(){
            return message.c_str();
        };
        private:
        std::string message;
    };

    //QueryBatchLessThan1Exception 请求的向量条目小于1
    class QueryBatchLessThan1Exception : public std::exception{
        public:
        QueryBatchLessThan1Exception() : message("query batch size less than 1") {};
        QueryBatchLessThan1Exception(std::string str) : message("query batch size less than 1: " + str) {};
        ~QueryBatchLessThan1Exception() throw(){};
        virtual const char *what() const throw(){
            return message.c_str();
        };
        private:
        std::string message;
    };
    //ConfigFileNotExistException 配置文件不存在
    class ConfigFileNotExistException : public std::exception{
        public:
        ConfigFileNotExistException() : message("config file not exist") {};
        ConfigFileNotExistException(std::string str) : message("config file not exist: " + str) {};
        ~ConfigFileNotExistException() throw(){};
        virtual const char *what() const throw(){
            return message.c_str();
        };
        private:
        std::string message;
    };

    //ConfigFileEmptyException 配置文件为空
    class ConfigFileNotExistException : public std::exception{
        public:
        ConfigFileNotExistException() : message("config file empty") {};
        ConfigFileNotExistException(std::string str) : message("config file empty: " + str) {};
        ~ConfigFileNotExistException() throw(){};
        virtual const char *what() const throw(){
            return message.c_str();
        };
        private:
        std::string message;
    };

    //CheckConfigSchemaException 配置格式不符合要求
    class CheckConfigSchemaException : public std::exception{
        public:
        CheckConfigSchemaException() : message("Config not valid") {};
        CheckConfigSchemaException(std::string str) : message("Config not valid: " + str) {};
        ~CheckConfigSchemaException() throw(){};
        virtual const char *what() const throw(){
            return message.c_str();
        };
        private:
        std::string message;
    };

    //IndexFileNotExistException index文件不存在
    class IndexFileNotExistException : public std::exception{
        public:
        IndexFileNotExistException() : message("Index file not exist") {};
        IndexFileNotExistException(std::string str) : message("Index file not exist: " + str) {};
        ~IndexFileNotExistException() throw(){};
        virtual const char *what() const throw(){
            return message.c_str();
        };
        private:
        std::string message;
    };
    //IndexDirNotExistException 保存index文件的文件夹不存在
    class IndexDirNotExistException : public std::exception{
        public:
        IndexDirNotExistException() : message("Index dir not exist") {};
        IndexDirNotExistException(std::string str) : message("Index dir not exist: " + str) {};
        ~IndexDirNotExistException() throw(){};
        virtual const char *what() const throw(){
            return message.c_str();
        };
        private:
        std::string message;
    };
    //IndexNameNotExistException index不存在
    class IndexNameNotExistException : public std::exception{
        public:
        IndexNameNotExistException() : message("Index name not exist") {};
        IndexNameNotExistException(std::string str) : message("Index name not exist: " + str) {};
        ~IndexNameNotExistException() throw(){};
        virtual const char *what() const throw(){
            return message.c_str();
        };
        private:
        std::string message;
    };
}