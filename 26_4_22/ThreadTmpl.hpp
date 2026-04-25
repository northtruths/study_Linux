// pthtead线程封装
//有模板传参版，必须传入参数
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

    template <class T>
    class ThreadTmpl 
    {
        using fun_t = std::function<void(const T &)>;
        static int number; // 线程编号

        enum class STATUS
        {
            NEW,
            RUNNING,
            STOP
        };

        static void *thread_func(void *arg)
        {
            ThreadTmpl<T> *thread = static_cast<ThreadTmpl<T> *>(arg);
            thread->_status = STATUS::RUNNING;
            while (thread->_status == STATUS::RUNNING)
                thread->_task(thread->_data);

            return nullptr;
        }

    public:
        // 线程构造（未实例创建，需启动），参数first为线程函数，second为用户任务的传参数据
        ThreadTmpl(fun_t task, const T &data = T()) : _task(task), _data(data), _status(STATUS::NEW), _is_detach(false)
        {
            _ptname = "pthread-" + std::to_string(number++);
            _pid = getpid();
        }
        ~ThreadTmpl() {}

        bool start()
        {
            if (_status == STATUS::NEW)
            {
                int n = pthread_create(&_pt, nullptr, thread_func, this);
                if (n)
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        bool cancel()
        {
            if (_status == STATUS::RUNNING)
            {
                _status = STATUS::STOP;
                return true;
            }
            return false;
        }

        std::string get_name()
        {
            return _ptname;
        }

        int wait(void **retval = nullptr)
        {
            if (_is_detach)
                return -1;
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

    template <typename T>
    int ThreadTmpl<T>::number = 1;

    // RAII管理线程对象
    template <typename T>
    class ThreadTmplGuard
    {
        using fun_t = std::function<void(const T &)>;
    public:
        ThreadTmplGuard(fun_t task, const T &data = T()) : _t(task, data), _is_wait(false)
        {
            _t.start();
        }

        ~ThreadTmplGuard()
        {
            _t.cancel();
            if (!_is_wait)
                _t.wait();
        }

        bool cancel()
        {
            return _t.cancel();
        }

        std::string get_name()
        {
            return _t.get_name();
        }

        int wait(void **retval = nullptr)
        {
            _is_wait = true;
            return _t.wait(retval);
        }

        int detach()
        {
            return _t.detach();
        }

        int get_pid()
        {
            return _t.get_pid();
        }

    private:
        LoopingThread<T> _t;
        bool _is_wait;
    };
}