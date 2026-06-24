#pragma once

#include "InetAddr.hpp"
#include "Log.hpp"
#include <string>
#include <iostream>
#include "protocol.hpp"

using namespace log_module;
using namespace protocol_module;

bool handle(std::string &msg, std::string &strin)
{
    std::stringstream ss(strin);
    std::string sx, sy, sopt;
    ss >> sx >> sopt >> sy;

    if (sopt.size() > 1)
        return false;
    // stoi转化非法数据会崩溃，所以捕获异常，但先不写
    int x = std::stoi(sx);
    int y = std::stoi(sy);
    char opt = sopt[0];
    Request rq(x, y, opt);
    rq.serialize(msg);
    Pack(msg);

    return true;
}

class TcpClient
{
public:
    TcpClient(std::string ip, uint16_t port)
        : server_addr_(ip, port)
    {
    }
    ~TcpClient() {}

    void init()
    {
        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd_ < 0)
        {
            LOG(LogLevel::FATAL) << "socket failure";
        }

        int n = connect(server_fd_, (sockaddr *)&server_addr_.sockaddr(), sizeof(server_addr_.sockaddr()));
        if (n < 0)
        {
            LOG(LogLevel::ERROR) << "connect failure";
            running_ = false;
        }
        else
        {
            LOG(LogLevel::INFO) << "connect succes... server_fd: " << server_fd_;
        }
    }

    void start()
    {
        while (running_)
        {
            // 获取用户输入
            std::string strin;
            std::cout << "请输入: [x] [opt] [y]" << std::endl;
            std::getline(std::cin, strin);
            // 发送数据
            std::string msg;
            if (handle(msg, strin) == false)
            {
                continue;
            }
            send(server_fd_, msg.c_str(), msg.size(), 0);
            // 接收解析返回数据
            parse();
        }
    }

    void parse()
    {
        char temp[1024];
        int n = recv(server_fd_, temp, sizeof(temp), 0);
        if (n < 0)
        {
            LOG(LogLevel::ERROR) << "recv failure";
        }
        temp[n] = 0;
        std::string buffer = temp;
        while (!buffer.empty())
        {
            Response rs;
            std::string msg;
            if(!UnPack(msg, buffer)){
                break;
            }
            rs.deserialize(msg);
            int state = rs.get_state();
            int result = rs.get_result();
            std::cout << "state:" << state << " " << "result:" << result << std::endl;
        }
    }

private:
    InetAddr server_addr_;

    int server_fd_ = -1;
    bool running_ = true;
};