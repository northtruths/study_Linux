#include<signal.h>
#include<cstdio>
#include<functional>
#include<vector>
#include<unistd.h>
#include<iostream>

using func = std::function<void()>;
std::vector<func> vf;

int count = 0;
void hanlder(int signal){
    vf[count++ % vf.size()]();
    alarm(2);
}

int main(){
    vf.push_back([](){std::cout << "执行任务1" << std::endl;});
    vf.push_back([](){std::cout << "执行任务2" << std::endl;});
    vf.push_back([](){std::cout << "执行任务3" << std::endl;});

    signal(SIGALRM, hanlder);
    alarm(2);
    while(true)
        ;
    return 0;
}