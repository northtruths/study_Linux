#pragma once
// 线程池 = 任务队列 + 多线程
// 任务生产->user->[任务队列 ->(锁) 多线程]
// 任务在线程池中的要求：短、快、一次执行

#include "Thread.hpp"
#include "Mutex.hpp"
#include "Cond.hpp"
#include <queue>
#include <functional>
#include <vector>

namespace threadpool_module
{
    template <typename T>
    class ThreadPool
    {
        using task_t = std::function<void(const T &)>;

        // 再底层才是真正的线程函数，所以这只是任务函数，不需要线程函数的格式
        void thread_task()
        {
            task_t t;
            T d;
            while (true)
            {
                {
                    mutex_module::FlexibleLock lock(_mutex);
                    while (is_empty() && _isrunning)
                    {
                        _cond.wait(lock);
                    }
                    if (is_empty() && !_isrunning)
                    {
                        break;
                    }
                    t = std::move(_task_queue.front());
                    _task_queue.pop();
                    d = std::move(_data_queue.front());
                    _data_queue.pop();
                }
                t(d);
            }
        }

        bool is_empty()
        {
            return _task_queue.empty();
        }

    public:
        ThreadPool(int num) : _treadnum(num), _isrunning(true)
        {
            for (int i = 0; i < _treadnum; ++i)
            {
                _tread_group.emplace_back(thread_module::ThreadPlainGuard(std::bind(&ThreadPool<T>::thread_task, this)));
            }
        }
        ~ThreadPool() {}

        bool push_task(const task_t &task, const T &data)
        {
            mutex_module::LockGuard lock(_mutex);
            if (_isrunning)
            {
                _task_queue.push(std::move(task));
                _data_queue.push(std::move(data));

                _cond.signal();
                return true;
            }
            else
            {
                return false;
            }
        }

        bool stop()
        {
            if (_isrunning)
            {
                _isrunning = false;
                _cond.broadcast();
                return true;
            }
            else
            {
                return false;
            }
        }

        void wait(){
            for(auto& t : _tread_group){
                t.wait();
            }
        }

    private:
        int _treadnum;
        bool _isrunning;

        std::queue<task_t> _task_queue;
        std::queue<T> _data_queue;
        std::vector<thread_module::ThreadPlainGuard> _tread_group;
        mutex_module::Mutex _mutex;
        cond_module::Cond _cond;
    };
};