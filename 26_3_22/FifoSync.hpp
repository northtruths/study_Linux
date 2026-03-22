#pragma once

#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

class FifoSync
{
public:
    FifoSync() : _fd(-1)
    {
    }
    ~FifoSync()
    {
        if (_fd != -1)
            close(_fd);
        if (_iscreater)
        {
            unlink("./fifo");
            std::cout << "管道清理成功" << std::endl;
        }
    }

    void Signal()
    {
        if (mkfifo(_path, 0666) < 0)
            perror("mkfifo error");
        _fd = ::open(_path, O_WRONLY | O_CREAT, 0666);
        if (_fd < 0)
            perror("open error");
        int flag = 1;
        write(_fd, &flag, sizeof(flag));
        _iscreater = true;
        std::cout << "发送成功" << std::endl;
    }

    void Waitblock()
    {
        int signal = 0;
        _fd = ::open(_path, O_RDONLY, 0666);
        if (_fd < 0)
            perror("open error");

        while (!signal)
            read(_fd, &signal, sizeof(signal));
        std::cout << "开始接收数据..." << std::endl;
    }

    bool WaitNonblock()
    {
        if(_count)
        {
            int flags = fcntl(_fd, F_GETFL, 0);
            fcntl(_fd, F_SETFL, flags | O_NONBLOCK);
            _count = 0;
        }

        int signal = 0;
        int flag = read(_fd, &signal, 1);
        if(flag < 0){
            if(errno != EAGAIN) {
                perror("Waitblock error");
            return false;
            }
        }        
        return (bool)flag;
    }

private:
    const char *_path = "./fifo";
    int _fd;
    bool _iscreater = false;
    int _count = 1;
};
