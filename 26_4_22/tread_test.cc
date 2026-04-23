#include<iostream>
#include<unistd.h>
#include"Thread.hpp"

void task(){
    std::cout << "任务测试" << std::endl;
}

int main(){
    thread_module::Thread<int> t;
    t.start();
    sleep(3);
    t.cancel();
    if(t.wait() == 0)
        std::cout << "回收成功" << std::endl;
    else    
        std::cout << "回收失败" << std::endl;
    return 0;
}