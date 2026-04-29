#pragma once
// 线程池 = 任务队列 + 多线程
// 任务生产->user->[任务队列 ->(锁) 多线程]
// 任务在线程池中的要求：短、快、一次执行

#include "ThreadPlain.hpp"
#include "Mutex.hpp"
#include "Cond.hpp"
#include <queue>
#include <functional>
#include <vector>
#include "Log.hpp"

namespace threadpool_module
{
    template <typename T>
    class ThreadPool
    {
        using task_t = std::function<void(const T &)>;

        // 再底层才是真正的线程函数，所以这只是任务函数，不需要线程函数的格式
        void thread_task(std::string name)
        {
            task_t t;
            T d;
            while (true)
            {
                {
                    mutex_module::FlexibleLock lock(_mutex);
                    while (is_empty() && _isrunning)
                    {
                        LOG(log_module::LogLevel::DEBUG) << name << " thread_task: 开始等待...";
                        _cond.wait(lock);
                        LOG(log_module::LogLevel::DEBUG) << name << "thread_task: 等待结束";
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
                LOG(log_module::LogLevel::DEBUG) << name << " thread_task: 开始工作";
                t(d);
                LOG(log_module::LogLevel::DEBUG) << name << " thread_task: 工作结束";
            }
        }

        bool is_empty()
        {
            return _task_queue.empty();
        }

    public:
        ThreadPool(int num) : _threadnum(num), _isrunning(true)
        {
            _thread_group.reserve(_threadnum);
            for (int i = 1; i <= _threadnum; ++i)
            {
                _thread_group.emplace_back(std::bind(&ThreadPool<T>::thread_task, this, "worker-" + std::to_string(i)));
            }
            LOG(log_module::LogLevel::DEBUG) << "构造结束";
        }

        bool push_task(const task_t &task, const T &data)
        {
            LOG(log_module::LogLevel::DEBUG) << "push_task: 准备发布任务";
            mutex_module::FlexibleLock lock(_mutex);
            if (_isrunning)
            {
                _task_queue.push(std::move(task));
                _data_queue.push(std::move(data));
                LOG(log_module::LogLevel::DEBUG) << "push_task: 任务发布成功";
                _cond.signal();
                return true;
            }
            else
            {
                LOG(log_module::LogLevel::WARNING) << "push_task: 线程池已停止";
                return false;
            }
        }

        bool stop()
        {
            mutex_module::FlexibleLock lock(_mutex);
            if (_isrunning)
            {
                _isrunning = false;
                LOG(log_module::LogLevel::ERROR) << "stop: 线程池已设置停止";
                _cond.broadcast();
                return true;
            }
            else
            {
                LOG(log_module::LogLevel::ERROR) << "stop: 线程池并非运行状态";
                return false;
            }
        }

        void wait()
        {
            for (auto &t : _thread_group)
            {
                t.wait();
            }
            LOG(log_module::LogLevel::DEBUG) << "wait: 所有线程wait成功";
        }

    private:
        int _threadnum;
        bool _isrunning;

        std::queue<task_t> _task_queue;
        std::queue<T> _data_queue;
        std::vector<thread_module::ThreadPlainGuard> _thread_group;
        mutex_module::Mutex _mutex;
        cond_module::Cond _cond;
    };
};