#include <iostream>
#include <unistd.h>
#include "ThreadTmpl.hpp"
#include "ThreadPlain.hpp"

void task(int)
{
    std::cout << "任务测试" << std::endl;
    sleep(1);
}

void task2()
{
    for(int i = 1; i <= 10; ++i)
    {
        std::cout << "任务测试" << i << std::endl;
        usleep(500000);
    }
}

int main()
{
    // thread_module::ThreadTmpl<int> t(task);
    // t.start();
    // sleep(3);
    // t.cancel();
    // if(t.wait() == 0)
    //     std::cout << "回收成功" << std::endl;
    // else
    //     std::cout << "回收失败" << std::endl;

    // std::cout << "任务2开始..." << std::endl;
    // thread_module::ThreadTmplGuard<int> tg(task);

    thread_module::ThreadPlain t(task2);
    // t.start();
    // sleep(3);
    // t.cancel();
    // if (t.wait() == 0)
    //     std::cout << "回收成功" << std::endl;
    // else
    //     std::cout << "回收失败" << std::endl;

    // std::cout << "任务2开始..." << std::endl;
    thread_module::ThreadPlainGuard tg(task2);
    sleep(6);
    return 0;
}