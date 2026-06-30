#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <functional>
#include "InetAddr.hpp"
#include "Socket.hpp"
#include "Log.hpp"

using namespace log_module;

using Task = std::function<void (const SockPtr&, const InetAddr&)>;

class TcpServer
{
public:
    TcpServer(Task task, uint16_t port)
        : running_(false), task_(task)
    {
        listen_socket_.build_tcpserver(port);
    }

    void start()
    {
        running_ = true;
        while (running_)
        {
            InetAddr client_addr;
            auto client_socket = listen_socket_.accept_socket(client_addr);
            if(client_socket == nullptr){
                continue;
            }
            pid_t n = ::fork();
            if(n < 0){
                LOG(LogLevel::ERROR) << "fork 失败";
                return;
            }else if(n == 0){
                //子进程
                listen_socket_.close_socket();
                if(::fork()){
                    ::exit(0);
                }
                //分离了孙子孤儿进程，可以单独进行IO和业务处理，不阻塞父进程
                task_(client_socket, client_addr);
                ::exit(0);
            }
            ::waitpid(n, nullptr, 0);
        }
    }

    ~TcpServer() {}

private:
    TcpSocket listen_socket_;
    bool running_;

    Task task_;
};