#include "Common.hpp"
#include"FifoSync .hpp"

int main(){
    cm;
    //1.共享空间获取
    cm.ShmGet();

    //2.挂接到进进程地址空间
    cm.ShmAttach();

    //3.通过命名管道实现进程同步（保护）
    FifoSync fs;

    //4.通信
    char* recv;
    recv = cm.GetAddr();
    fs.Waitblock();
    while(fs.WaitNonblock()){
        printf("%s\n", recv);
        sleep(1);
    }
    
    return 0;
}