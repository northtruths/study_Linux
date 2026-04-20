// POSIX信号量sem的封装
#pragma once

#include <semaphore.h>

namespace sem_module
{
    class Sem
    {
        Sem(const Sem &) = delete;
        Sem &operator=(const Sem &) = delete;

    public:
        Sem(int value)
        {
            int r = sem_init(&_sem, 0, value);
            (void)r;
        }
        ~Sem()
        {
            sem_destroy(&_sem);
        }

        int wait()
        {
            int r = sem_wait(&_sem);
            return r;
        }

        int post()
        {
            int r = sem_post(&_sem);
            return r;
        }

    private:
        sem_t _sem;
    };
}