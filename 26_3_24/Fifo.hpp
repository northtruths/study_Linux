#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include<sys/types.h>
#include<cstdio>
#include<fcntl.h>
#include<cstring>
#include<unistd.h>

#define PATH "./fifo"

class Fifo{
public:
    Fifo(){
        umask(0);
        mkfifo(PATH, 0666);
    };
    ~Fifo(){
        close(_fd);
        if(_iscearter){
            unlink(PATH);
        }
    };

    void Write(const void *in){
        _fd = open(PATH, O_WRONLY, 0666);
        if(_fd < 0)
            perror("write error");
        write(_fd, in, strlen((char*) in));
        _iscearter = true;
    }

    void Read(void *buf){
        _fd = open(PATH, O_RDONLY);
        if(_fd < 0)
            perror("read error");
        read(_fd, buf, 1024);
    }
private:
    int _fd;
    bool _iscearter = false;
};

Fifo ff;