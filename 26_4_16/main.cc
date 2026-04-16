//通过互斥锁实现互斥的抢票程序  练习
#include"Tickets.hpp"
#include<vector>

#define INIT_TICKETS 20 //初始票数
#define PTHREAD_SIZE 5  //抢票程序个数（线程数）

int nums = INIT_TICKETS;//派发数量

int main(){
    //std::vector<Tickets> tickets(PTHREAD_SIZE, Tickets(nums));
    std::vector<tks::Tickets> tickets;
    tickets.reserve(PTHREAD_SIZE);
    for(int i = 0; i < PTHREAD_SIZE; ++i){
        tickets.emplace_back(nums);
    }
    sleep(1);//等待线程创建
    while(nums > 0){
        for(int i = 0; i < PTHREAD_SIZE; ++i)
            tickets[i].how_many();
        std::cout << "还剩" << nums << "张" << std::endl;
        sleep(1);
    }
    std::cout << "抢票结束" << std::endl;
    std::cout << "还剩" << nums << "张" << std::endl;
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