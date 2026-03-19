#include<iostream>
#include<string>
#include<vector>
#include <unistd.h>      
#include <sys/types.h> 
#include<cstdlib>

class Channel{
public:
    Channel();

    ~Channel();
private:
    int _num;
};

int main(int argc, char* argv[]){
    if(argc != 2)
        return 1;//参数无效
    
    int num = std::stoi(argv[1]);
    std::vector<Channel> chas;
    while(num--){
        int pipefd[2];
        if(::pipe(pipefd) < 0)
            return 2;//管道创建失败

        int pid = ::fork();
        if(pid == 0){
            close(pipefd[1]);//子进程读，关闭写端
            //执行任务
            exit(0);
        }

        close(pipefd[0]);//父进程写，关闭读端
    }
    return 0;
}