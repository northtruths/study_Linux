#pragma once
#include<string>

class Channel{
public:
    Channel(int fd, int who )
        :_wfd(fd)
        ,_who(who)
    {
        _name = "Channel_" + std::to_string(_wfd) + "_" + std::to_string(_who);
    }


    int GetWfd(){
        return _wfd;
    }
    
    int GetWho(){
        return _who;
    }

    const std::string& GetName(){
        return _name;
    }

    ~Channel(){}
private:
    int _wfd;//父进程所管理的某管道的写fd
    std::string _name;//管道名字，用wfd区分
    int _who;//子进程pid
};