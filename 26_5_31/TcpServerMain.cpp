
#include <signal.h>
#include <string>
#include "TcpServer.hpp"
#include "protocol.hpp"

using namespace protocol_module;

bool task(std::string &out_msg, std::string &buff)
{
    std::string msg;
    if (!UnPack(msg, buff))
    {
        return false;
    }
    Request rq;
    rq.deserialize(msg);
    int x = rq.get_x();
    int y = rq.get_y();
    char opt = rq.get_opt();

    int state = 0, result = 0;
    switch (opt)
    {
    case '+':
    {
        result = x + y;
        state = 0;
        break;
    }
    case '-':
    {
        result = x - y;
        state = 0;
        break;
    }
    case '*':
    {
        result = x * y;
        state = 0;
        break;
    }
    case '/':
    {
        if (y == 0)
        {
            state = 1;
        }
        else
        {
            result = x / y;
            state = 0;
        }
        break;
    }
    default:
        break;
    }
    Response rs(state, result);
    std::string rs_string;
    rs.serialize(rs_string);
    if(!Pack(rs_string)){
        return false;
    }
    out_msg = rs_string;
    return true;
}

int main()
{
    signal(SIGCHLD, SIG_IGN);
    TcpServer ts([](std::string &out_msg, std::string &buff)->bool{
        return task(out_msg, buff);
    });

    // 1.初始化服务器
    ts.init();

    // 2.启动并等待连接
    ts.start();

    // 3.等待数据并处理

    return 0;
}