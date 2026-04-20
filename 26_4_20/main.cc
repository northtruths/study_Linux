// 通过信号量实现线程安全的环形队列，最终实现生产者与消费者模型

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "RingQueue.hpp"

int data = 0;

void *productor(void *arg)
{
    ring_queue::RingQueue<int> *rq = static_cast<ring_queue::RingQueue<int> *>(arg);
    while (true)
    {
        std::cout << "生成数据:" << data << std::endl;//打印在外，未保护所以可能会打印混乱，可以加锁或者放push里
        rq->push(data++);
        sleep(1);//1s生产1个数据
    }
}

void *consumer(void *arg)
{
    ring_queue::RingQueue<int> *rq = static_cast<ring_queue::RingQueue<int> *>(arg);
    while (true)
    {
        int r = rq->pop();
        std::cout << "消费数据:" << r << std::endl;
        sleep(2);//2s消费1个数据，生产阻塞
    }
}

int main()
{
    ring_queue::RingQueue<int> rq(5);
    pthread_t p1,p2,p3;
    pthread_t c1,c2,c3;
    pthread_create(&p1, nullptr, productor, &rq);
    pthread_create(&c1, nullptr, consumer, &rq);
    pthread_create(&p2, nullptr, productor, &rq);
    pthread_create(&c2, nullptr, consumer, &rq);
    //pthread_create(&p3, nullptr, productor, &rq);
    //pthread_create(&c3, nullptr, consumer, &rq);


    pthread_join(p1, nullptr);
    pthread_join(c1, nullptr);
    return 0;
}