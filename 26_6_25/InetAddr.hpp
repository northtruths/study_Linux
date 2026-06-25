#pragma once
// 封装sockin_addr_in
//

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

class InetAddr
{
    void ip_ntoh()
    {
        char ipbuffer[64];
        ip_ = ::inet_ntop(AF_INET, &in_addr_.sin_addr, ipbuffer, sizeof(ipbuffer));
    }
    void port_ntoh()
    {
        port_ = ntohs(in_addr_.sin_port);
    }

public:
    InetAddr(){}

    //接受网络序列套接字地址
    InetAddr(const sockaddr_in &addr)
    {
        init(addr);
    }

    //任意接收ip和指定port（服务端）
    InetAddr(const uint16_t &port)
    {
        init(port);
    }

    //指定ip和指定port（客户端）
    InetAddr(const std::string &ip, const uint16_t &port)
    {
        init(ip, port);
    }

    void init(const sockaddr_in &addr){
        in_addr_ = addr;
        ip_ntoh();
        port_ntoh();
    }

    void init(const uint16_t &port){
        ip_ = "0.0.0.0";
        port_ = port;
        memset(&in_addr_, 0, sizeof(in_addr_));
        in_addr_.sin_family = AF_INET;
        in_addr_.sin_port = htons(port);
        in_addr_.sin_addr.s_addr = INADDR_ANY;
    }

    void init(const std::string &ip, const uint16_t &port){
        ip_ = ip;
        port_ = port;
        memset(&in_addr_, 0, sizeof(in_addr_));
        in_addr_.sin_family = AF_INET;
        in_addr_.sin_port = htons(port);
        in_addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    }

    ~InetAddr() {}

    in_addr_t IP_INET()
    {
        return inet_addr(ip_.c_str());
    }
    std::string &IP_HOST()
    {
        return ip_;
    }
    in_port_t PORT_INET()
    {
        return in_addr_.sin_port;
    }
    uint16_t PORT_HOST()
    {
        return port_;
    }

    sockaddr_in &sockaddr()
    {
        return in_addr_;
    }

private:
    // 网络序列
    sockaddr_in in_addr_;

    // 主机序列
    std::string ip_;
    uint16_t port_;
};