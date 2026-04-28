// pthtead线程封装
// 无模版无需传参版，参数在外部函数
#pragma once

#include <pthread.h>
#include <functional>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "Log.hpp"

namespace thread_module
{
    class ThreadPlain
    {
        using fun_t = std::function<void()>;
        static int number; // 线程编号

        static void *thread_func(void *arg)
        {
            ThreadPlain *thread = static_cast<ThreadPlain *>(arg);
            thread->_task();
            thread->_status = STATUS::STOP;
            return nullptr;
        }

    public:
        enum class STATUS
        {
            NEW,
            RUNNING,
            STOP,
            INVALID
        };
        ThreadPlain(const ThreadPlain &) = delete;
        ThreadPlain &operator=(const ThreadPlain &) = delete;

        // 线程构造（未实例创建，需启动），参数first为线程函数
        ThreadPlain(fun_t task) : _task(task), _status(STATUS::NEW), _is_detach(false)
        {
            _ptname = "pthread-" + std::to_string(number++);
            _pid = getpid();
        }

        ThreadPlain(ThreadPlain &&other) noexcept
            : _task(std::move(other._task)), _status(std::move(other._status)), _is_detach(other._is_detach), _pt(other._pt), _ptname(std::move(other._ptname)), _pid(other._pid)
        {
            other._status = STATUS::INVALID;
            other._pt = 0;
            other._pid = 0;
        }

        ThreadPlain &operator=(ThreadPlain &&other) noexcept
        {
            if (this != &other)
            {
                // 先释放当前资源
                if (_pt && !_is_detach)
                {
                    wait();
                }
                // 转移资源
                _task = std::move(other._task);
                _status = other._status;
                _pt = other._pt;
                _is_detach = other._is_detach;
                _ptname = std::move(other._ptname);
                _pid = other._pid;
                // 清空原对象
                other._status = STATUS::INVALID;
                other._pt = 0;
                other._pid = 0;
            }
            return *this;
        }

        ~ThreadPlain()
        {
            //LOG(log_module::LogLevel::DEBUG) << " ~ThreadPlain: 线程析构";
            if (_status == STATUS::INVALID)
            {
                //LOG(log_module::LogLevel::DEBUG) << " ~ThreadPlain: 临时对象析构";
            }
            else if (_status == STATUS::RUNNING)
            {
                cancel();
            }
            //LOG(log_module::LogLevel::DEBUG) << " ~ThreadPlain: 准备wait";
            wait();
            //LOG(log_module::LogLevel::DEBUG) << " ~ThreadPlain: 线程析构完成";
        }

        bool start()
        {
            if (_status == STATUS::NEW)
            {
                _status = STATUS::RUNNING;
                int n = pthread_create(&_pt, nullptr, thread_func, this);
                if (n)
                {
                    //LOG(log_module::LogLevel::ERROR) << "start: 线程创建失败";
                    return false;
                }
                //LOG(log_module::LogLevel::DEBUG) << "start: 线程创建成功";
                return true;
            }
            //LOG(log_module::LogLevel::WARNING) << "start: 线程状态并非新建(NEW)";
            return false;
        }

        bool cancel()
        {
            if (_status == STATUS::RUNNING)
            {
                pthread_cancel(_pt);
                _status = STATUS::STOP;
                return true;
            }
            //LOG(log_module::LogLevel::WARNING) << "cancle: 线程状态并非运行(RUNNING)";
            return false;
        }

        std::string get_name()
        {
            return _ptname;
        }

        int wait(void **retval = nullptr)
        {
            //LOG(log_module::LogLevel::DEBUG) << "wait: 判断是否分离";
            if (_is_detach)
                return -1;
            //LOG(log_module::LogLevel::DEBUG) << "wait: 判断完成";
            //LOG(log_module::LogLevel::DEBUG) << "wait: 准备join";
            int n;
            if(_pt)
                n =  pthread_join(_pt, retval);
            _pt = 0;                                               
            _status = STATUS::STOP;
            return n;
            //LOG(log_module::LogLevel::DEBUG) << "wait: join完成";
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

        STATUS get_status()
        {
            return _status;
        }

    private:
        fun_t _task;
        STATUS _status;
        bool _is_detach;

        pthread_t _pt;
        std::string _ptname;
        pid_t _pid;
    };
    int ThreadPlain::number = 1;

    // RAII管理线程对象
    class ThreadPlainGuard
    {
        using fun_t = std::function<void()>;

    public:
        ThreadPlainGuard(const ThreadPlainGuard &) = delete;
        ThreadPlainGuard operator=(const ThreadPlainGuard &) = delete;

        ThreadPlainGuard(fun_t task) : _t(task), _is_wait(false)
        {
            _t.start();
        }

        ThreadPlainGuard(ThreadPlainGuard &&other) : _t(std::move(other._t)), _is_wait(other._is_wait)
        {
            _t.start();
        }

        ThreadPlainGuard &operator=(ThreadPlainGuard &&other)
        {
            this->_t = std::move(other._t);
            this->_is_wait = other._is_wait;
            return *this;
        }

        ~ThreadPlainGuard()
        {
            //LOG(log_module::LogLevel::DEBUG) << " ~ThreadPlainGuard: RAII线程析构";
            if (_t.get_status() == ThreadPlain::STATUS::INVALID)
            {
                //LOG(log_module::LogLevel::DEBUG) << " ~ThreadPlainGuard: 临时对象析构";
            }
            else
            {
                if (_t.get_status() == ThreadPlain::STATUS::RUNNING)
                    _t.cancel();
                if (!_is_wait)
                    _t.wait();
            }
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
        ThreadPlain _t;
        bool _is_wait;
    };
}