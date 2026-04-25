#include<iostream>
#include<unistd.h>
#include"LoopingThread.hpp"

void task(int){
    std::cout << "任务测试" << std::endl;
    sleep(1);
}

int main(){
    thread_module::LoopingThread<int> t(task);
    t.start();
    sleep(3);
    t.cancel();
    if(t.wait() == 0)
        std::cout << "回收成功" << std::endl;
    else    
        std::cout << "回收失败" << std::endl;

    std::cout << "任务2开始..." << std::endl;
    thread_module::LoopingThreadGuard<int> tg(task);
    sleep(3);
    return 0;
}