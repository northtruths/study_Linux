#include<unistd.h>
#include<stdio.h>
#include<cstring>

int main(){
    int pipefd[2];
    pipe(pipefd);

    pid_t fd = fork();
    if(fd == 0){
        close(pipefd[1]);
        char buf[1024];
        read(pipefd[0], buf, 1024);
        printf("%s\n", buf);
    }else if(fd > 0){
        close(pipefd[0]);
        const char* send = "i am father";
        write(pipefd[1], send, strlen(send));
    }
    else{
        printf("错误！！\n");
    }
    return 0;
}