#include<signal.h>
#include<iostream>

void sigcb(int sig)
{
    std::cout << "触发本次事件的信号值为：" << sig << std::endl;
    exit(0);
}

int main(){
    struct sigaction sa;
    sa.sa_handler = sigcb;
    sigaction(SIGINT, &sa, nullptr);
    while(1){
        ;
    }
    return 0;
}