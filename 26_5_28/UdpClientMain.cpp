#include"UdpClient.hpp"
#include<iostream>

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cout << "./client " << "[ip] " << "[port]" << std::endl;
        return 0;
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);
    //1.客户端启动
    UdpClient udp_cli(serverip, serverport);
    udp_cli.init();
    //2.发送信息
    udp_cli.start();
    return 0;
}