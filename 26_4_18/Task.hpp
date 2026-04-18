// 生产者消费者模型的数据传输任务类
#include "BlockingQueue.hpp"
#include<unistd.h>
#include <queue>

namespace task
{
    template <typename T>
    class Task
    {
    public:
        Task(int cap) : running(true), _pc(cap), _dq(10)
        {
            pthread_create(&proc, nullptr, proc_active, (void *)this);
            pthread_create(&cons, nullptr, cons_active, (void *)this);
        }
        ~Task()
        {
            _pc.wakeup_all();
            _dq.wakeup_all();
            std::cout << "任务完成，正常退出" << std::endl;
        }

        void add_data(const T &data)
        {
            if (running)
                _dq.push(data);
            else
            {
                std::cout << "任务已停止，无法继续发送数据！！" << std::endl;
            }
        }

        void quit()
        {
            running = false;
        }

    private:
        static void *proc_active(void *arg)
        {
            Task *_this = static_cast<Task *>(arg);
            while (_this->running)
            {
                T data = _this->_dq.pop();
                if (!_this->running)
                    break;
                _this->_pc.push(data);
                if (!_this->running)
                    break;
                std::cout << "数据已生成：" << data << std::endl;
            }
            return nullptr;
        }

        static void *cons_active(void *arg)
        {
            Task *_this = static_cast<Task *>(arg);
            while (_this->running)
            {
                T ret = _this->_pc.pop();
                if (!_this->running)
                    break;
                std::cout << "消费者拿到了：" << ret << std::endl;
            }
            return nullptr;
        }

    private:
        block_queue::BlockingQueue<T> _pc; // 生产者|消费者阻塞队列
        block_queue::BlockingQueue<T> _dq; // 数据生成|生产者阻塞队列
        pthread_t proc;
        pthread_t cons;
        bool running;
    };
}