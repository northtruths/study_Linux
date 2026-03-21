#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <sys/wait.h>
#include <functional>
#include <time.h>
#include "Task.hpp"
#include "Channel.hpp"

#define WORKMAX 20

enum
{
    OK = 0,
    UsageError,
    PipeError,
    ForkError
};


class ProcessPool
{
public:
    ProcessPool(int num)
        :_num(num)
    {
        init_pool();
    }

    int child_task()
    {
        for (auto &c : _chans)
        {
            close(c.GetWfd()); // 关闭由继承父进程文件描述符表所指向前面管道多余的fd
        }
        while (true)
        {
            int n = 0; // 任务命令号
            int flag = read(0, &n, sizeof(n));
            if (flag < 0)
                return PipeError; // 管道读取失败
            if (flag == 0)
                break;
            tl.DoWork(n);
        }
        exit(0);
    }

    int init_pool()
    {
        for (int i = 0; i < _num; ++i)
        {
            int pipefd[2];
            if (::pipe(pipefd) < 0)
                return PipeError; // 管道创建失败

            int pid = ::fork();
            if (pid == 0)
            {
                // 子进程执行任务
                close(pipefd[1]); // 子进程读，关闭写端
                dup2(pipefd[0], 0);
                child_task();
            }
            _chans.emplace_back(pipefd[1], pid);
            close(pipefd[0]); // 父进程写，关闭读端
        }
        return 0;
    }

    void dispatch_task()
    {
        int count = 0; // 派发任务次数
        srand((unsigned)time(NULL));
        while (true)
        {
            int n = rand() % tl.WorkNum() + 1;
            write(_chans[count++ % _num].GetWfd(), &n, sizeof(n));
            // 子进程重定向0让可以直接写入，包装
            sleep(1);
            std::cout << "任务已还剩" << WORKMAX - count << "项" << std::endl;
            if (count == WORKMAX)
                break;
        }
    }

    int destroy_pool()
    {
        // 逐个回收可能因为某些子进程提前退出导致close逻辑混乱失败，但因为实验性代码只作说明不做修改
        for (auto &c : _chans)
        {
            if (close(c.GetWfd()) == -1)
                return PipeError; // 关闭通道失败
            int status;
            pid_t pid = waitpid(-1, &status, 0);
            if (pid == -1)
                return 5; // 回收子进程失败
            std::cout << "成功回收子进程：" << c.GetWho() << std::endl;
        }
        return 0;
    }

    void DebugCreat()
    {
        for (auto &c : _chans)
        {
            std::cout << c.GetName() << std::endl;
        }
    }

private:
    std::vector<Channel> _chans;
    int _num;
};