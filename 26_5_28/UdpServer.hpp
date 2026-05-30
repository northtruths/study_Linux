#pragma once

#include "Log.hpp"
#include "InetAddr.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<iostream>
#include<string>

using namespace log_module;

u_int16_t gport = 8888;
class UdpServer
{
public:
    UdpServer(u_int16_t port = gport)
        : server_addr_(port)
    {}
    ~UdpServer() {}
    void init()
    {
        fd_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd_socket_ == -1)
        {
            LOG(LogLevel::FATAL) << "socket建立失败!";
        }
        LOG(LogLevel::INFO) << "socket建立成功, fd: " << fd_socket_;
        int n = bind(fd_socket_, (sockaddr *)&server_addr_.sockaddr(), sizeof(server_addr_.sockaddr()));
        if (n == -1)
        {
            LOG(LogLevel::FATAL) << "bind失败!";
        }
        LOG(LogLevel::INFO) << "bind成功, n: " << n;
    }
    void start()
    {
        struct sockaddr_in temp = {};
        while (running)
        {
            socklen_t len = sizeof(temp);
            char buff[1024] = {};
            int n = recvfrom(fd_socket_, &buff, sizeof(buff), 0, (sockaddr *)&temp, &len);
            if(n == -1){
                perror("recvfrom error");
                LOG(LogLevel::WARNING) << "客户端信息接受失败";
            }
            buff[n] = 0;
            InetAddr client_addr(temp);
            
            std::string client_info = client_addr.IP_HOST() + ":" + std::to_string(client_addr.PORT_HOST());
            std::cout << client_info << " say: " << buff << std::endl;

            std::string back = "server back: " ;
            back += buff;
            sendto(fd_socket_, back.c_str(), back.size(), 0, (sockaddr*)&client_addr.sockaddr(), (socklen_t)sizeof(client_addr.sockaddr()));
        }
    }

private:
    InetAddr server_addr_;
    bool running = true;
    int fd_socket_ = -1;
};