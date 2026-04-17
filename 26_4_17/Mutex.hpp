#pragma once

#include <mutex>
#include"Cond.hpp"//头文件相互包含，暂时不解决

namespace mutex_module
{
    class Mutex
    {
    public:
        Mutex(const Mutex &) = delete;
        Mutex &operator=(const Mutex &) = delete;
        Mutex()
        {
            int r = pthread_mutex_init(&_mutex, nullptr);
            (void)r;
        }
        ~Mutex()
        {
            pthread_mutex_destroy(&_mutex);
        }

        int lock()
        {
            return pthread_mutex_lock(&_mutex);
        }

        int unlock()
        {
            return pthread_mutex_unlock(&_mutex);
        }

        pthread_mutex_t& get_mutex(){
            return _mutex;
        }

    private:
        pthread_mutex_t _mutex;
    };

    //管理“锁的使用过程”（RAII）
    class LockGuard
    {
    public:
        LockGuard(Mutex& mutex):_lock(mutex)
        {
            _lock.lock();
        }
        ~LockGuard(){
            _lock.unlock();
        }
    private:
        Mutex& _lock;
    };

    //能使用cond的灵活锁
    class FlexibleLock{
    public:
        FlexibleLock(Mutex& mutex):_lock(mutex),_islocked(false)
        {
            lock();
        } 
        ~FlexibleLock(){
            if(_islocked){
                _lock.unlock();
            }
        }

        int lock(){
            if(!_islocked){
                _islocked = true;
                return _lock.lock();
            }
            return 1;
        }

        int unlock(){
            if(_islocked){
                _islocked = false;
                return _lock.unlock();
            }
            return 1;
        }

        pthread_mutex_t* native(){
            return &_lock.get_mutex();
        }
    private:
        Mutex& _lock;
        bool _islocked;//对锁的控制权
    };
}
