#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <unistd.h>

using task = std::function<void()>;

void work1()
{
    std::cout << getpid() << "已完成任务1" << std::endl;
}

void work2()
{
    std::cout << getpid() << "已完成任务2" << std::endl;
}

void work3()
{
    std::cout << getpid() << "已完成任务3" << std::endl;
}

class TaskList
{
public:
    TaskList()
        : tasks({work1, work2, work3})
        , _num(tasks.size())
    {}

    int DoWork(int n){
        if(n >= _num)
            return 0;
        tasks[n]();
        return 0;
    }

    int WorkNum(){
        return _num;
    }

    ~TaskList() {}

private:
    std::vector<task> tasks;//任务列表
    int _num;//任务数量
};

TaskList tl;