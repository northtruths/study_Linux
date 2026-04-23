// pthtead线程封装
#pragma once

#include <pthread.h>
#include <functional>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

// 创建、销毁、wait、push任务（functional）,start,stop,data(name),分离
namespace thread_module
{
    static int number = 1; // 线程编号

    template <class T>
    class Thread
    {
        using fun_t = std::function<void(const T &)>;

        enum class STATUS
        {
            NEW,
            RUNNING,
            STOP
        };

        static void default_task(const T &)
        {
            // 默认调试任务
            std::cout << "任务成功" << std::endl;
            sleep(1);
        }

        static void *thread_func(void *arg)
        {
            Thread<T> *thread = static_cast<Thread<T> *>(arg);
            thread->_status = STATUS::RUNNING;
            while (thread->_status == STATUS::RUNNING)
                thread->_task(thread->_data);
            return nullptr;
        }

    public:
        Thread(fun_t task = default_task, const T &data = T()) : _task(task), _data(data), _status(STATUS::NEW), _is_detach(false)
        {
            _ptname = "pthread-" + std::to_string(number++);
            _pid = getpid();
        }
        ~Thread() {}

        bool start()
        {
            if (_status == STATUS::NEW)
            {
                int n = pthread_create(&_pt, nullptr, thread_func, this);
                if (n)
                {
                    std::cout << "线程创建失败" << std::endl;
                    return false;
                }
                return true;
            }
            std::cout << "start失败" << std::endl;
            return false;
        }

        bool cancel()
        {
            if (_status == STATUS::RUNNING)
            {
                int n = pthread_cancel(_pt);
                if (n)
                {
                    std::cout << "线程取消失败" << std::endl;
                    return false;
                }
                _status = STATUS::STOP;
                return true;
            }
            std::cout << "stop失败" << std::endl;
            return false;
        }

        std::string get_name()
        {
            return _ptname;
        }

        int wait(void **retval = nullptr)
        {
            return pthread_join(_pt, retval);
        }

        int detach()
        {
            _is_detach = true;
            return pthread_detach(_pt);
        }

        int get_pid()
        {
            return _pid;
        }

    private:
        fun_t _task;
        T _data;
        STATUS _status;
        bool _is_detach;

        pthread_t _pt;
        std::string _ptname;
        pid_t _pid;
    };
}