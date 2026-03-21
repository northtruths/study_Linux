#include"Channel.hpp"
#include"ProcessPool.hpp"
#include"Task.hpp"

int main(int argc, char* argv[]){
    if(argc != 2)
        return 1;//参数无效
    
    //1.初始化进程池
    int num = std::stoi(argv[1]);
    ProcessPool pp(num);

    //2.派发任务
    pp.dispatch_task();

    //3.关闭进程池
    pp.destroy_pool();

    return 0;
}