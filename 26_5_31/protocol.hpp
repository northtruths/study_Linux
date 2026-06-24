#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <jsoncpp/json/json.h>

std::string Sep = "\r\n";

// 将传入的msg打包
bool Pack(std::string &msg)
{
    //  \r\n作为分隔符，[size]\r\n[Jsondata]\r\n 作为一个完整的协议格式
    if (msg.empty())
    {
        return false;
    }
    size_t size = msg.size();
    msg = std::to_string(size) + Sep + msg + Sep;
    return true;
}

// 解包一条完整信息给msg
bool UnPack(std::string &msg, std::string &buff)
{
    size_t end = buff.find(Sep);
    if (end == std::string::npos)
    {
        return false;
    }
    size_t size = std::stoi(buff.substr(0, end));
    if (buff.size() < end + size + 2 * Sep.size())
    {
        return false;
    }
    msg = buff.substr(end + Sep.size(), size);
    buff.erase(0, end + size + 2 * Sep.size());
    return true;
}

class request
{
public:
    request()
        : x_(0), y_(0), opt_(0)
    {
    }
    request(int x, int y, char opt)
        : x_(x), y_(y), opt_(opt)
    {
    }

    bool serialize(std::string &out_str)
    {
        Json::Value root;
        root["x"] = x_;
        root["y"] = y_;
        root["opt"] = opt_;
        Json::StreamWriterBuilder buider;
        std::unique_ptr<Json::StreamWriter> pw(buider.newStreamWriter());
        std::stringstream ss;
        pw->write(root, &ss);
        out_str = ss.str();
        return true;
    }

    bool deserialize(std::string &in_str)
    {
        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(in_str, root))
        {
            std::cout << "Json parse failure: " << reader.getFormattedErrorMessages() << std::endl;
            return false;
        }

        x_ = root["x"].asInt();
        y_ = root["y"].asInt();
        opt_ = root["opt"].asInt();
        return true;
    }

    int get_x()
    {
        return x_;
    }

    int get_y()
    {
        return y_;
    }

    char get_opt()
    {
        return opt_;
    }

private:
    int x_;
    int y_;
    char opt_;
};

class response
{
public:
    response()
        : state_(0), result_(0)
    {
    }
    response(int state, int result)
        : state_(state), result_(result)
    {
    }

    bool serialize(std::string &out_str)
    {
        Json::Value root;
        root["state"] = state_;
        root["result"] = result_;
        Json::StreamWriterBuilder buider;
        std::unique_ptr<Json::StreamWriter> pw(buider.newStreamWriter());
        std::stringstream ss;
        pw->write(root, &ss);
        out_str = ss.str();
        return true;
    }

    bool deserialize(std::string &in_str)
    {
        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(in_str, root))
        {
            std::cout << "Json parse failure: " << reader.getFormattedErrorMessages() << std::endl;
            return false;
        }

        state_ = root["state"].asInt();
        result_ = root["result"].asInt();
        return true;
    }

    int get_state()
    {
        return state_;
    }

    int get_result()
    {
        return result_;
    }

private:
    int state_; // 状态码，0为有效
    int result_;
};