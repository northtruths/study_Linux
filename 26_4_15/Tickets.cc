//通过互斥锁实现互斥的抢票程序  练习
#include<iostream>
#include<pthread.h>
#include<mutex>
#include<string>
#include<unistd.h>

#define INIT_TICKETS 20 //初始票数
#define PTHREAD_SIZE 5  //线程数

int num = INIT_TICKETS;//派发数量

class Tickets{
public:
    Tickets()
        :_num(0)
    {
        pthread_create(&_pt, nullptr, grap_tickets, this);
    }
    ~Tickets(){}

    static void* grap_tickets(void* p){
        Tickets* t = static_cast<Tickets*>(p);
        t->init_name();
        while(num > 0){
            sleep(1);//增加超卖概率
            --num;
            ++t->_num;
            sleep(1);
        }
        return nullptr;
    }

    void init_name(){
        _name = "t" + std::to_string(pthread_self());
    }

    void how_many(){
        std::cout << _name << "抢到了" << _num << "张票" << std::endl;
    }

    pthread_t getid(){
        return pthread_self();
    }
private:
    pthread_t _pt;
    std::string _name;
    int _num;//抢到的票数
};

int main(){
    Tickets tickets[PTHREAD_SIZE];

    while(1){
        for(int i = 0; i < PTHREAD_SIZE; ++i)
            tickets[i].how_many();
        std::cout << "还剩" << num << "张" << std::endl;
        sleep(1);
    }
    for(auto& t : tickets){
        int n = pthread_join(t.getid(), nullptr);
        if(n != 0){
            std::cout << "回收失败" << std::endl;
        }
    }
    std::cout << "全部回收成功" << std::endl;
    return 0;
}