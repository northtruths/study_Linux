//通过互斥锁实现互斥的抢票程序  练习
#include<iostream>
#include<pthread.h>
#include<mutex>
#include<string>
#include<unistd.h>

#define INIT_TICKETS 20 //初始票数
#define PTHREAD_SIZE 5  //线程数

int nums = INIT_TICKETS;//派发数量

class Tickets{
public:
    Tickets()
        :_nums(0)
    {
        pthread_create(&_pt, nullptr, grap_tickets, this);
    }
    ~Tickets(){}

    static void* grap_tickets(void* p){
        Tickets* t = static_cast<Tickets*>(p);
        t->init_name();
        while(nums > 0){
            sleep(1);//增加超卖概率
            --nums;
            ++t->_nums;
            sleep(1);
        }
        return nullptr;
    }

    void init_name(){
        _name = "t" + std::to_string(pthread_self());
    }

    void how_many() const{
        std::cout << _name << "抢到了" << _nums << "张票" << std::endl;
    }

    pthread_t getid() const{
        return _pt;
    }
private:
    pthread_t _pt;
    std::string _name;
    int _nums;//抢到的票数
};

int main(){
    Tickets tickets[PTHREAD_SIZE];

    while(nums > 0){
        for(int i = 0; i < PTHREAD_SIZE; ++i)
            tickets[i].how_many();
        std::cout << "还剩" << nums << "张" << std::endl;
        sleep(1);
    }
    for(int i = 0; i < PTHREAD_SIZE; ++i)
            tickets[i].how_many();
    for(auto& t : tickets){
        int n = pthread_join(t.getid(), nullptr);
        if(n != 0){
            std::cout << "回收失败" << std::endl;
        }
    }
    std::cout << "全部回收成功" << std::endl;
    return 0;
}