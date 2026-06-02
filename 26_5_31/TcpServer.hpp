#pragma once

#include "InetAddr.hpp"
#include "Log.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <functional>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace log_module;

using Task = std::function<void(std::string)>;
uint16_t gport = 8888;

class TcpServer
{

    void handle(Task task, std::string client_data, InetAddr client_addr, int client_fd)
    {
        if (task)
        {
        }
        else
        {
            // 默认任务
            std::cout << client_addr.IP_HOST() << ':' << client_addr.PORT_HOST() << " say: ";
            std::cout << client_data;

            std::string rbuff = "server say: " + client_data;
            // LOG(LogLevel::DEBUG) << "服务端接收数据为: " << client_data;
            send(client_fd, rbuff.c_str(), rbuff.size(), 0);
        }
    }

public:
    TcpServer(Task task = nullptr, uint16_t port = gport)
        : task_(task), server_addr_(port)
    {
    }

    void init()
    {
        listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd_ < 0)
        {
            LOG(LogLevel::FATAL) << "socket失败";
        }
        bind(listen_fd_, (sockaddr *)&server_addr_.sockaddr(), sizeof(server_addr_.sockaddr()));
        listen(listen_fd_, 10);
        LOG(LogLevel::INFO) << "开始listen... listen_fd: " << listen_fd_;
    }

    // 单执行流版
    //  void start()
    //  {
    //      while (true)
    //      {
    //          struct sockaddr_in temp;
    //          memset(&temp, 0, sizeof(temp));
    //          socklen_t len = sizeof(temp);
    //          int client_fd = accept(listen_fd_, (sockaddr *)&temp, &len);
    //          if (client_fd < 0)
    //          {
    //              LOG(LogLevel::FATAL) << "accept失败";
    //          }
    //          InetAddr client_addr(temp);
    //          LOG(LogLevel::INFO) << "accepted: " << client_addr.IP_HOST() << ':' << client_addr.PORT_HOST() << " client_fd: " << client_fd;

    //         while (true)
    //         {
    //             char buff[1024];
    //             int n = recv(client_fd, buff, sizeof(buff), 0);
    //             if (n == 0)
    //                 break;
    //             if (n < 0)
    //             {
    //                 LOG(LogLevel::ERROR) << "recv failure";
    //             }
    //             buff[n] = 0;
    //             // 处理
    //             handle(task_, buff, client_addr, client_fd);
    //         }
    //     }
    // }

    // 多进程版
    void start()
    {
        while (true)
        {
            struct sockaddr_in temp;
            memset(&temp, 0, sizeof(temp));
            socklen_t len = sizeof(temp);
            int client_fd = accept(listen_fd_, (sockaddr *)&temp, &len);
            if (client_fd < 0)
            {
                LOG(LogLevel::FATAL) << "accept失败";
            }
            pid_t pid = fork();
            if (pid == -1)
            {
                LOG(LogLevel::FATAL) << "fork失败";
            }
            // 子进程会继承与客户端的通信fd，父进程不再需要与客户端的通信fd，子进程不需要父进程的监听fd

            // 子进程
            if (pid == 0)
            {
                close(listen_fd_);
                InetAddr client_addr(temp);
                LOG(LogLevel::INFO) << "accepted: " << client_addr.IP_HOST() << ':' << client_addr.PORT_HOST() << " client_fd: " << client_fd;
                while (true)
                {
                    char buff[1024];
                    int n = recv(client_fd, buff, sizeof(buff), 0);
                    if (n == 0)
                        break;
                    if (n < 0)
                    {
                        LOG(LogLevel::ERROR) << "recv failure";
                    }
                    buff[n] = 0;
                    // 处理
                    handle(task_, buff, client_addr, client_fd);
                }
                close(client_fd);
                exit(0);
            }

            // 父进程
            close(client_fd);
        }
    }

    ~TcpServer() {}

private:
    Task task_;
    InetAddr server_addr_;

    int listen_fd_ = -1;
    bool running_ = true;
};