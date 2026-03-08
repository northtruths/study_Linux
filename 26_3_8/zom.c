#include<stdio.h>
#include<unistd.h>

int main(){
    pid_t id = 0;
    id = fork();
    if(id){
        while(1){
            sleep(1);
            printf("我是父进程，pid为%d，ppid为%d\n", getpid(), getppid());
        }
    } 
    else if(id == 0){
        printf("我是子进程，pid为%d，ppid为%d\n", getpid(), getppid());
        sleep(5);
    }
    else{
        printf("创建失败");
    }

    return 0;
}
