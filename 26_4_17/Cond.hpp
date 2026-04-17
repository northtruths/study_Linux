//条件变量的封装
#pragma once

#include<pthread.h>
#include"Mutex.hpp"//头文件相互包含，暂时不解决


namespace cond_module{

    class Cond{
    public:
        Cond(){
            pthread_cond_init(&_cond, nullptr);
        }
        ~Cond(){
            pthread_cond_destroy(&_cond);
        }

        int wait(mutex_module::FlexibleLock& lock){
            return pthread_cond_wait(&_cond, lock.native());
        }

        int signal(){
            return pthread_cond_signal(&_cond);
        }

        int broadcast(){
            return pthread_cond_broadcast(&_cond);
        }
    private:
        pthread_cond_t _cond;
    };
}