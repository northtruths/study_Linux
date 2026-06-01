
#include "TcpServer.hpp"

int main(){
    TcpServer ts;

    //1.初始化服务器
    ts.init();

    //2.启动并等待连接
    ts.start();
    
    //3.等待数据并处理

    return 0;
}