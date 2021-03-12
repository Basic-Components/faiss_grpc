#include <iostream>
#include <random>
#include <memory>
#include <utility>
#include <string>
#include <cstdlib>
#include <filesystem>
// #include <pthread.h>
#include <map>
#include <exception>
// #include <libfswatch/libfswatch.h>
#include <faiss/index.h>
#include <faiss/index_io.h>
using faiss::Index;
using faiss::read_index;
namespace fs = std::filesystem;

// 64-bit int
using idx_t = faiss::Index::idx_t;


namespace faiss_index_manager{
    //endsWith c++20才可以替换掉
    bool endsWith(std::string s,std::string sub){
        return s.rfind(sub)==(s.length()-sub.length())? true:false;
    }
    //DNotEqualException 维度不一致异常
    class DNotEqualException : public std::exception{
        public:
            DNotEqualException() : message("d not equal"){}
            DNotEqualException(std::string str) : message("d not equal," + str) {}
            ~DNotEqualException() throw () {
            }

            virtual const char* what() const throw () {
                return message.c_str();
            }

        private:
            std::string message;
    };

    class IndexManager{
        public:
            ~IndexManager(){
                std::cout<<"destructor called!"<<std::endl;
            }
            IndexManager(const IndexManager&)=delete;
            IndexManager& operator=(const IndexManager&)=delete;
            static IndexManager& get_instance(){
                static IndexManager instance;
                return instance;

            }
            //load_index 加载指定路径下的index文件
            //@params std::vector< std::string > paths 查找index文件的路径列表
            void load_index(std::vector< std::string > paths){
                for (auto ps : paths){
                    for(auto& p: fs::directory_iterator(ps)){
                        auto ppath = p.path();
                        auto s = p.symlink_status();
                        auto name = ppath.filename().string();
                        if (fs::is_regular_file(s) && faiss_index_manager::endsWith(name,".index")) {
                            auto indexname = ppath.filename().stem().string();
                            std::cout << "find faiss index --" << indexname << "-- in " << ppath << "\n" << std::endl;
                            (this->m)[indexname]= std::unique_ptr<Index>(read_index(ppath.string().c_str()));
                            std::cout << "load faiss index" << ppath << "ok\n" << std::endl;
                        }
                    }         
                }
            }
            //search 查找指定index中与请求最相近的前k个向量id
            //@params std::string indexname index名
            //@params std::vector<std::vector<float>> query 查找的向量列表
            //@params int k 查找最相似的k个向量id
            //@return std::vector<std::vector<long long>> 请求向量列表对应的排名id列表,第一层与query对应,第二层顺序为排名
            //@exception std::out_of_range 未找到 indexname则会抛出超界错误
            //@exception faiss_index_manager::DNotEqual query的d和index的d不一致则会抛出DNotEqual
            std::vector<std::vector<long long>> search(std::string indexname, std::vector<std::vector<float>> query ,int k){
                auto nq = query.size();
                auto d = query.front().size();
                idx_t* I = new idx_t[k * nq];
                float* D = new float[k * nq];
                float* xq = new float[d * nq];
                for (int i = 0; i < nq; i++) {
                    for (int j = 0; j < d; j++){
                        xq[d * i + j] = query[i][j];
                    }     
                }
                std::cout << "init test ok/n" << std::endl;
                (this->m).at(indexname);
                if ((this->m)[indexname]->d != d){
                    throw faiss_index_manager::DNotEqualException();
                }
                (this->m)[indexname]->search(nq, xq, k, D, I);
                std::cout << "search test ok/n" << std::endl;
                std::vector<std::vector<long long>> result;
                printf("I=\n");
                for (int i = 0; i < nq; i++) {
                    std::vector<long long> v;
                    for (int j = 0; j < k; j++){
                        printf("%5zd ", I[i * k + j]);
                        v.push_back(I[i * k + j]);
                    }
                    printf("\n ");
                    result.push_back(v);
                }
                delete [] I;
                delete [] D;
                delete [] xq;
                return result;
            }
        private:
            std::map<std::string,std::unique_ptr<Index>> m;
            IndexManager(){
                std::cout<<"constructor called!"<<std::endl;
            }
    };
}


using faiss_index_manager::IndexManager;

int main(int argc, char** argv) {
    std::cout << "start testting" << std::endl;
    std::vector< std::string > paths = {"./"};
    IndexManager& instance = IndexManager::get_instance();
    instance.load_index(paths);
    //测试
    int d = 64;      // dimension
    int nq = 10;  // nb of queries

    std::mt19937 rng;
    std::uniform_real_distribution<> distrib;
    float* xq = new float[d * nq];
    std::vector<std::vector<float>> query;
    for (int i = 0; i < nq; i++) {
        std::vector<float> line;
        for (int j = 0; j < d; j++){
            auto v = distrib(rng);
            xq[d * i + j] = v;
            line.push_back(v);
        }
        query.push_back(line);
    }

    int k = 4;
    std::string indexname = "example";
    auto res = instance.search(indexname,query,k);
    std::cout << "start testting" << std::endl;
    return 0;
}