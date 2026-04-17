//练习条件变量cond的基本使用
#include<pthread.h>
#include<iostream>
#include<unistd.h>
#include<signal.h>
#include"Mutex.hpp"

mutex_module::Mutex mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool running = 1;//进程运行状态
int space = 1;//生产空间,只有一个
int count = 0;//计数AB一共执行次数

void* active1(void* arg){

    while(running){
        {
            mutex_module::LockGuard lock(mutex);
            while(space == 0 && running)
                pthread_cond_wait(&cond, &lock.get_mutex());//看看是不是能和互斥锁换个位置，主要想知道如果发送信号唤醒时，条件变量下为空或者消费者因为事情都还没到会发生什么，信号丢失？
            if(!running)
                    break;
                std::cout << "我是" << (char*)arg << std::endl;
            space = 0;
            ++count;
            pthread_cond_signal(&cond);
        }
    }

    return nullptr;
}

void* active2(void* arg){

    while(running){
        {
            mutex_module::LockGuard lock(mutex);
            while(space == 1 && running)
                pthread_cond_wait(&cond, &lock.get_mutex());
            if(!running)
                break;
            std::cout << "我是" << (char*)arg << std::endl;
            space = 1;
            ++count;
            pthread_cond_signal(&cond);
        }
    }

    return nullptr;
}

int main(){
    signal(SIGALRM, [](int sig){
        running = false;
        pthread_cond_broadcast(&cond);
    }
    );
    //线程A作为生产者，线程B作为消费者
    pthread_t A;
    pthread_t B;
    pthread_create(&A, nullptr, active1, (void*)"线程A");
    pthread_create(&B, nullptr, active2, (void*)"线程B");

    alarm(1);//1s后回收线程
    int a = pthread_join(A, nullptr);
    int b = pthread_join(B, nullptr);
    if(a || b)
        std::cout << "回收异常" << std::endl;
    else    
        std::cout << "回收成功" << std::endl;
    std::cout << count << std::endl;
    return 0;
}