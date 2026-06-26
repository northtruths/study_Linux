#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <memory>
#include <unistd.h>
#include "InetAddr.hpp"
#include "Log.hpp"
#include "Common.hpp"

using namespace log_module;

class Socket;
using SockPtr = std::shared_ptr<Socket>;

class Socket
{
public:
    virtual ~Socket() = default;
    virtual bool init_socket() = 0;
    virtual bool set_socket() = 0;
    virtual bool bind_socket(uint16_t port) = 0;
    virtual bool listen_socket() = 0;
    virtual SockPtr accept_socket(InetAddr &out_client) = 0;
    virtual ssize_t send_socket(int cilent_fd, const std::string &msg) = 0;
    virtual int recv_socket(std::string *out) = 0;
    virtual bool close_socket() = 0;
    virtual int get_fd() = 0;

    void build_tcpserver(uint16_t port = gdefaultport)
    {
        init_socket();
        set_socket();
        bind_socket(port);
        listen_socket();
    }
};

// 对于监听socket，可以使用build_tcpserver构造，对于通信socket只能通过构造参数构造
class TcpSocket : public Socket
{
public:
    TcpSocket()
        : fd_(gdefaultfd)
    {
    }
    TcpSocket(int fd) : fd_(fd)
    {
    }
    virtual ~TcpSocket()
    {
        if (fd_ == gdefaultfd)
        {
            return;
        }
        else
        {
            close_socket();
        }
    }
    virtual bool init_socket() override
    {
        if (fd_ != gdefaultfd)
        {
            LOG(LogLevel::WARNING) << "socket 已初始化";
            return false;
        }
        fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ < 0)
        {
            LOG(LogLevel::ERROR) << "socket 失败";
            return false;
        }
        LOG(LogLevel::DEBUG) << "socket 创建成功: " << fd_;
    }

    virtual bool set_socket() override
    {
        // 配置socket再断开后端口可以马上再连接使用
        int opt = 1;
        int n = ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if (n < 0)
        {
            LOG(LogLevel::ERROR) << "set socket 失败";
            return false;
        }
        return true;
    }

    virtual bool bind_socket(uint16_t port) override
    {
        if (port == gdefaultport)
        {
            LOG(LogLevel::WARNING) << "未指定bind端口";
            return false;
        }
        InetAddr addr(port);
        int n = bind(fd_, (sockaddr *)&addr.sockaddr(), sizeof(addr.sockaddr()));
        if (n < 0)
        {
            LOG(LogLevel::ERROR) << "bind 失败";
            return false;
        }
    }

    virtual bool listen_socket() override
    {
        if (fd_ == gdefaultfd)
        {
            LOG(LogLevel::ERROR) << "socket 未初始化";
            return false;
        }
        int n = listen(fd_, glisten_num);
        if (n < 0)
        {
            LOG(LogLevel::ERROR) << "listen 失败";
            return false;
        }
    }

    virtual SockPtr accept_socket(InetAddr &out_client) override
    {
        sockaddr_in cli;
        socklen_t len = sizeof(cli);
        int client_fd = accept(fd_, (sockaddr *)&cli, &len);
        if (client_fd < 0)
        {
            LOG(LogLevel::WARNING) << "accept 失败";
            return nullptr;
        }
        out_client.init(cli);
        return std::make_shared<TcpSocket>(client_fd);
    }

    virtual ssize_t send_socket(int client_fd, const std::string &msg) override
    {
        return send(client_fd, msg.c_str(), msg.size(), 0);
    }

    virtual int recv_socket(std::string *out) override
    {
        if (out == nullptr)
        {
            LOG(LogLevel::ERROR) << "接收缓冲区无效";
            return -1;
        }
        char buff[1024];
        int n = recv(fd_, buff, sizeof(buff) - 1, 0);
        if (n < 0)
        {
            LOG(LogLevel::ERROR) << "recv 失败";
            return n;
        }
        else if (n == 0)
        {
            LOG(LogLevel::INFO) << "客户端fd:" << fd_ << "断开连接";
            return n;
        }
        buff[n] = 0;
        *out = buff;
        return n;
    }

    virtual bool close_socket() override
    {
        if (fd_ == gdefaultfd)
        {
            return false;
        }
        ::close(fd_);
    }
    virtual int get_fd() override
    {
        if (fd_ == gdefaultfd)
        {
            LOG(LogLevel::WARNING) << "fd 未初始化";
            return -1;
        }
        return fd_;
    }

private:
    int fd_;
};