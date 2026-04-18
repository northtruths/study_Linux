//通过条件变量+互斥锁实现线程安全的阻塞队列，最终实现生产者与消费者模型(具体实现中多了一个数据生成和生产者的关系实现)
#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<ctime>
#include<signal.h>
#include"Task.hpp"

bool running = true;

task::Task<int>* pt;
void quit(int sig){
    pt->quit();
    running = false;
}

void work(){
    task::Task<int> t1(5);
    pt = &t1;
    //sleep(3);//让生产者和消费者都等3秒
    while(running){
        t1.add_data(rand());
        sleep(1);
    }
}

int main(){
    srand((unsigned int)time(nullptr));

    signal(SIGALRM, quit);
    alarm(5);

    work();
    return 0;
}