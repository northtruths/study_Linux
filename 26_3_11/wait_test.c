#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>

int main(){
    pid_t id = 0;
    id = fork();
    if(id == 0){
        printf("i am child process\n");
        sleep(5);
    }
    else if(id){
        int status = 0;
        wait(&status);
        if(WIFEXITED(status)){
            printf("正常退出\n");
            printf("coredump：%d\n", WCOREDUMP(status));
            printf("退出码：%d\n", WEXITSTATUS(status));
        }else{
            printf("异常退出\n");
        }
    }
    return 0;
}
