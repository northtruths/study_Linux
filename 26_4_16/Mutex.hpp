#pragma once

#include <mutex>

namespace lock_module
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

    private:
        pthread_mutex_t _mutex;
    };

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
}
