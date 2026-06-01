#include"TcpClient.hpp"

std::string gip = "127.0.0.1";
uint16_t gport = 8888;

int main(){
    TcpClient tc(gip, gport);
    //1.初始化客户端+建立与服务端的连接
    tc.init();

    //2.获取用户输入并发送
    tc.start();

    return 0;
}