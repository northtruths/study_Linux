#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include"Task.hpp"

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
        : tasks({nullptr, work1, work2, work3})
        , _num(tasks.size() - 1)
    {}

    void DoWork(int n){
        tasks[n]();
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