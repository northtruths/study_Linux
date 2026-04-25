
#include "ThreadPool.hpp"
#include "Thread.hpp"
#include <iostream>
#include <unistd.h>

void task(int i)
{

    std::cout << "任务已完成，共完成 " << i << " 次任务" << std::endl;
    sleep(1);
}

int main()
{
    threadpool_module::ThreadPool<int> tpool(5);
    int t = 10;
    int i = 1;
    while (t--)
    {
        std::cout << "准备push" << std::endl;
        tpool.push_task(task, i++);
        std::cout << "push成功" << std::endl;
    }

    return 0;
}