#include<stdio.h>
#include<unistd.h>

int main(){
    pid_t pid = fork();
    if(pid){
        //父进程退出
        return 0;
    }else{
        //子进程进入僵尸状态
        sleep(60);
    }
    return 0;
}
