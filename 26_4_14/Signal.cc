#include<signal.h>
#include<iostream>
#include<unistd.h>

void sigcb(int sig){
    std::cout << "触发本次事件的信号值为：" << sig << std::endl;
}

int main(){
    signal(SIGINT, sigcb);
    while(1){
        sleep(1);
    }
    return 0;
}