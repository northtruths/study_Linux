#include<iostream>
#include<string>
#include<vector>
#include <unistd.h>      
#include <sys/types.h> 
#include<cstdlib>
#include<sys/wait.h>
#include<functional>
#include<time>
#include"Task.hpp"

class Channel{
public:
    Channel(const int fd)
        :_wfd(fd)
    {
        _name = "Channel_" + std::to_string(_wfd);
    }

    ~Channel();
private:
    int _wfd;//父进程所管理的某管道的写fd
    std::string _name;//管道名字，用wfd区分
};



 
int main(int argc, char* argv[]){
    if(argc != 2)
        return 1;//参数无效
    
    //1.初始化进程池
    int num = std::stoi(argv[1]);
    std::vector<Channel> chas;
    while(num--){
        int pipefd[2];
        if(::pipe(pipefd) < 0)
            return 2;//管道创建失败

        int pid = ::fork();
        if(pid == 0){
            close(pipefd[1]);//子进程读，关闭写端
            while(true){
                int n = 0;
                read(pipefd[0], &n, sizeof(n));
                tm[n];
            }
            exit(0);
        }
        chas.emplace_back(pipefd[1]); 
        close(pipefd[0]);//父进程写，关闭读端
    }

    //2.派发任务
    int count = 0;//派发任务次数
    srand((unsigned)time(NULL));
    while(true){
        write(chas[count++%num], srand(), sizeof(srand()));
        sleep(1);
        if(count == 20)
            break;
    }

    int status = 0;
    ::waitpid(-1, &status, 0);
    //3.关闭进程池
    return 0;
}