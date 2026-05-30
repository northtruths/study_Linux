#pragma once
//封装sockaddr_in

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<string>
#include<cstring>


class InetAddr{
    void ip_ntoh(){
        char ipbuffer[64];
        ip_ = ::inet_ntop(AF_INET, &addr_.sin_addr, ipbuffer, sizeof(ipbuffer));
    }
    void port_ntoh(){
        port_ = ntohs(addr_.sin_port);
    }

public:
    InetAddr(sockaddr_in addr)
        :addr_(addr)
    {
        ip_ntoh();
        port_ntoh();
    }

    InetAddr(uint16_t port)
        :port_(port)
    {
        memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port_);
        addr_.sin_addr.s_addr = INADDR_ANY;
    }

    InetAddr(std::string ip, uint16_t port)
        :ip_(ip), port_(port)
    {
        memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port_);
        addr_.sin_addr.s_addr = inet_addr(ip_.c_str());
    }
    
    ~InetAddr(){}

    in_addr_t IP_INET(){
        return inet_addr(ip_.c_str());
    }
    std::string& IP_HOST(){
        return ip_;
    }
    uint16_t PORT_HOST(){
        return port_;
    }
    in_port_t PORT_INET(){
        return addr_.sin_port;
    }
    sockaddr_in& sockaddr(){
        return addr_;
    }
private:
    //网络序列
    sockaddr_in addr_;

    //主机序列
    std::string ip_;
    uint16_t port_;
};