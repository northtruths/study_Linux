#pragma once

#include "Log.hpp"
#include "InetAddr.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include<iostream>
#include<string>

using namespace log_module;

class UdpClient
{
public:
    UdpClient(std::string ip, uint16_t port)
        :server_addr_(ip, port)
    {}
    ~UdpClient() {}
    void init()
    {
        fd_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd_socket_ == -1)
        {
            LOG(LogLevel::FATAL) << "客户端socket建立失败!";
        }
        LOG(LogLevel::FATAL) << "客户端socket建立成功, fd: " << fd_socket_;
    }
    void start()
    {
        char buff[1024] = {};
        while (running)
        {
            std::cout << "请输入: ";
            std::cin >> buff;
            ssize_t n = sendto(fd_socket_, buff, sizeof(buff), 0, (sockaddr*)&server_addr_.sockaddr(), sizeof(server_addr_.sockaddr()));
            if(n == -1){
                std::perror("sendto");
                LOG(LogLevel::ERROR) << "发送失败";
            }

            struct sockaddr_in temp;
            socklen_t len = sizeof(temp);
            char rback[1024] = {};
            recvfrom(fd_socket_, rback, sizeof(rback), 0, (sockaddr*)&temp, &len);
            std::cout << rback << std::endl;
        }
    }


private:
    InetAddr server_addr_;
    bool running = true;
    int fd_socket_ = -1;
};