#include"UdpServer.hpp"
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<cstring>

int main(){
    UdpServer udp_svr;
    //1.初始化
    udp_svr.init();

    //2.启动(等待信息并处理)
    udp_svr.start();

    return 0;
}