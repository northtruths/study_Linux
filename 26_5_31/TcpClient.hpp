#pragma once

#include "InetAddr.hpp"
#include "Log.hpp"
#include<string>
#include<iostream>

using namespace log_module;

class TcpClient{
public:
    TcpClient(std::string ip, uint16_t port)
        :server_addr_(ip, port)
    {}
    ~TcpClient(){}

    void init(){
        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if(server_fd_ < 0){
            LOG(LogLevel::FATAL) << "socket failure";
        }

        int n = connect(server_fd_, (sockaddr*)&server_addr_.sockaddr(), sizeof(server_addr_.sockaddr()));
        if(n < 0){
            LOG(LogLevel::ERROR) << "connect failure";
            running_ = false;
        }
        else{
            LOG(LogLevel::INFO) << "connect succes... server_fd: " << server_fd_;
        }
    }

    void start(){
        while(running_){
            //获取用户输入
            std::string strsend;
            std::cout << "请输入: ";
            std::getline(std::cin, strsend);
            strsend += '\n';
            //发送数据
            send(server_fd_, strsend.c_str(), strsend.size(), 0);
            //接收返回数据
            handle();
        }
    }

    void handle(){
        char buff[1024];
        int n = recv(server_fd_, buff, sizeof(buff), 0);
        if(n < 0){
            LOG(LogLevel::ERROR) << "recv failure";
        }
        buff[n] = 0;

        std::cout << buff;
    }
private:
    InetAddr server_addr_;

    int server_fd_ = -1;
    bool running_ = true;
};