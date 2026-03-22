#include "Common.hpp"
#include"FifoSync .hpp"

int main(){
    umask(0);
    cm;
    //1.共享空间创建
    cm.ShmCreat();

    //2.挂接到进进程地址空间
    cm.ShmAttach();

    //3.通信
    char* send;
    send = cm.GetAddr();
    char in = 'A';
    while(in <= 'Z'){
        send[in - 'A'] = in;
        ++in;
    }

    //4.通过命名管道实现进程同步（保护）
    FifoSync fs;
    fs.Signal();

    sleep(10);
    //5.去挂接，回收空间
    cm.Destory();
    return 0;
}