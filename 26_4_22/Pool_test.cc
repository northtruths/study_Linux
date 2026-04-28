
#include "ThreadPool.hpp"
#include "ThreadPlain.hpp"
#include <iostream>
#include <unistd.h>
#include"Log.hpp"


void task(int i)
{
    std::cout << "任务已完成，共完成 " << i << " 次任务" << std::endl;
}

int main()
{
    threadpool_module::ThreadPool<int> tpool(5);
    int t = 3;
    int i = 1;
    while (t--)
    {
        std::cout << "外部准备push" << std::endl;
        tpool.push_task(task, i++);
        std::cout << "push成功" << std::endl;
        sleep(1);
    }
    tpool.stop();
    tpool.wait();
    //sleep(5);
    return 0;
}