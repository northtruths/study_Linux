// 通过信号量实现线程安全的环形队列，最终实现生产者与消费者模型
#pragma once

#include <vector>
#include <pthread.h>
#include <vector>
#include "Mutex.hpp"
#include "Sem.hpp"

namespace ring_queue
{
    template <typename T>
    class RingQueue
    {
    public:
        RingQueue(int value) : _ring(value), _sem_proc(value), _sem_cons(0), _proc_pos(0), _cons_pos(0), _cap(value)
        {
        }
        ~RingQueue() {}

        void push(const T &data)
        {
            _sem_proc.wait();
            {
                mutex_module::LockGuard lock(_mutex_proc);
                _ring[_proc_pos] = data;
                _proc_pos = (_proc_pos + 1) % _cap;
            }
            _sem_cons.post();
        }

        T pop()
        {
            _sem_cons.wait();
            T data;
            {
                mutex_module::LockGuard lock(_mutex_cons);
                data = _ring[_cons_pos];
                _cons_pos = (_cons_pos + 1) % _cap;
            }
            _sem_proc.post();
            return data;
        }

    private:
        std::vector<T> _ring;
        sem_module::Sem _sem_proc;
        sem_module::Sem _sem_cons;
        int _proc_pos;
        int _cons_pos;
        int _cap;
        mutex_module::Mutex _mutex_proc;
        mutex_module::Mutex _mutex_cons;
    };
}