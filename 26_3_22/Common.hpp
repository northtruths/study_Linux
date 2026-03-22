#pragma once

#include<sys/ipc.h>
#include<sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/stat.h>
#include<cstdio>
#include<iostream>
#include<fcntl.h>

#define SSIZE 4096
#define KEYID 123


class Common{
public:
    Common()
        :_shmid(-1)
        ,_addr((void*) -1)
    {}

    ~Common(){

    }

    key_t Getkey(){
        return ftok("./Common.hpp", KEYID);
    }

    void ShmCreat(){
        _shmid = shmget(Getkey(), SSIZE, IPC_CREAT | IPC_EXCL | 0666);
        if( _shmid < 0)
            perror("ShmCreat error");
        else
            std::cout << "创建成功" << std::endl;

    }

    void ShmGet()
    {
        _shmid = shmget(Getkey(), 0, 0666);
        if (_shmid < 0)
            perror("ShmGet error");
        else
            std::cout << "获取成功" << std::endl;
    }

    int GetShmid(){
        return _shmid;
    }

    void ShmAttach(){
        _addr = shmat(_shmid, nullptr, 0);
        if(_addr == (void*)-1)
            perror("ShmAttach error");
        else
           std::cout << "挂接成功" << std::endl;
    }

    char* GetAddr(){
        return (char*)_addr;
    }

    void Destory(){
        shmdt(_addr);
        shmctl(_shmid, IPC_RMID, NULL);
        std::cout << "清理成功" << std::endl;
    }
private:
    int _shmid;
    void* _addr;
};

Common cm;