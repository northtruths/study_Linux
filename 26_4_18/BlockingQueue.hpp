// 阻塞队列
#include <queue>
#include "Mutex.hpp"
#include "Cond.hpp"
#include <iostream>

namespace block_queue
{
    template <typename T>
    class BlockingQueue
    {
    public:
        BlockingQueue(int cap) : _cap(cap), _size(0),_running(true)
        {
        }
        ~BlockingQueue() {}

        void push(const T &data)
        {
            mutex_module::FlexibleLock lock(mutex);
            while (_size == _cap && _running)
            {
                std::cout << "生产者等待中..." << std::endl;
                _proc_cond.wait(lock);
            }
            if(!_running)
                return;
            std::cout << "生产者醒了" << std::endl;
            _queue.push(data);
            ++_size;
            _cons_cond.signal();
        }

        T pop()
        {
            mutex_module::FlexibleLock lock(mutex);
            while (_size == 0 && _running)
            {
                std::cout << "消费者等待中..." << std::endl;
                _cons_cond.wait(lock);
            }
            if(!_running)
                return T();
            std::cout << "消费者醒了" << std::endl;
            T ret = _queue.front();
            _queue.pop();
            --_size;
            _proc_cond.signal();
            return ret;
        }

        void wakeup_all(){
            _proc_cond.broadcast();
            _cons_cond.broadcast();
            _running = false;
        }

    private:
        int _cap;  // 总容量
        int _size; // 当前数据数量
        bool _running;
        std::queue<T> _queue;
        mutex_module::Mutex mutex;
        cond_module::Cond _proc_cond; // 容量为空
        cond_module::Cond _cons_cond; // 容量为满

    };
}