#include<signal.h>
#include<iostream>

int main(){
    sigset_t set, old;
    sigaddset(&set, 2);
    sigaddset(&set, 40);
    sigprocmask(SIG_BLOCK, &set, &old);
    while(1){
        ;
    }
    std::cout << "运行结束" << std::endl;
    return 0;
}