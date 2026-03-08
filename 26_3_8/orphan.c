#include<stdio.h>
#include<unistd.h>

int main(){
    pid_t id = 0;
    id = fork();
    if(id){
        printf("我是父进程，pid：%d，ppid：%d\n", getpid(), getppid());
        sleep(5);
    }
    else if(id == 0){
        while(1){
           printf("我是子进程，pid：%d，ppid：%d\n", getpid(), getppid());
           sleep(1);
        }
    }
    return 0;
}
