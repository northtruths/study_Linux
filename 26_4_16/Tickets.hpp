// 抢票类
#pragma once

#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include "Mutex.hpp"

namespace tks
{
    lock_module::Mutex mutex;

    class Tickets
    {
        int &_nums; // 外部票的数量
    public:
        Tickets(int &_nums)
            : _own(0), _nums(_nums)
        {
            pthread_create(&_pt, nullptr, grap_tickets, this);
        }
        ~Tickets() {}

        static void *grap_tickets(void *p)
        {
            Tickets *t = static_cast<Tickets *>(p);
            t->init_name();

            while (true)
            {
                {
                    lock_module::LockGuard lock(mutex);
                    if (t->_nums > 0)
                    {
                        --t->_nums;
                        ++t->_own;
                    }
                    else
                    {
                        break;
                    }
                }
                sleep(1); // 降低抢锁频率
            }

            return nullptr;
        }

        void init_name()
        {
            _name = "t" + std::to_string(pthread_self());
        }

        void how_many() const
        {
            std::cout << _name << "抢到了" << _own << "张票" << std::endl;
        }

        pthread_t getid() const
        {
            return _pt;
        }

    private:
        pthread_t _pt;
        std::string _name;
        int _own; // 抢到的票数
    };
}
