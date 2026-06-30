#pragma once

#include "HttpProtocol.hpp"
#include "TcpServer.hpp"
#include "Log.hpp"
#include <functional>
#include <string>
#include <fstream>


class HttpServer
{
public:
    HttpServer(int port)
        : tcp_(port)
    {
    }
    void start()
    {
        tcp_.init([this](const SockPtr &client_socket, const InetAddr &clinet_addr)
                  { handle_client(client_socket, clinet_addr); });
        tcp_.start();
    }


private:
    void handle_client(const SockPtr &client_socket, const InetAddr &clinet_addr)
    {
        // 1. 获取请求
        std::string msg;
        int n = client_socket->recv_socket(&msg);
        (void)n;
        LOG(LogLevel::DEBUG) << "请求获取成功";

        // 2.解析请求
        Request req;
        req.deserialize(msg);

        // 3. 处理业务，目前只客户端请求post方法
        int code = 200;
        std::string body = get_content(req, code);

        // 4. 构造响应
        Response resp;
        resp.set_version(req.get_version());
        resp.set_status(code);
        resp.set_body(body);
        //  设置报头行，目前没设置

        LOG(LogLevel::DEBUG) << "业务以处理";

        // 5. 发送响应
        std::string raw_response = resp.serialize();
        client_socket->send_socket(raw_response);
        LOG(LogLevel::DEBUG) << "已对客户端响应";
    }
    
    std::string get_content(const Request &req, int &code)
    {
        std::string path = root + req.get_uri();

        if (req.get_uri() == "/")
        {
            path = root + "/index.html";
        }

        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            path = root + "/404.html";
            file.open(path, std::ios::binary);
            if (!file.is_open())
            {
                return "<h1>404 Not Found</h1>";
            }
            code = 404;
        }

        return std::string((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
    }

private:
    TcpServer tcp_;
};