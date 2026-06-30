#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Common.hpp"

// 请求行 -    【请求方法】 【空格】 【URI】 【空格】 【HTTP版本】 【换行符】
// 请求报头 -  【Key】【:】【空格】【Value】 【换行符】
// 请求报头...
// 空行 -      【换行符】
// 请求正文 - 【DATA】
class Request
{
public:
    bool serialize(std::string &out_data)
    {
        out_data.clear();
        // 1. 请求行：GET /index.html HTTP/1.1
        out_data += method_ + " " + uri_ + " " + version_ + "\r\n";
        // 2. 请求报头
        for (const auto &[key, value] : headers_)
        {
            out_data += key + ": " + value + "\r\n";
        }
        // 3. 空行
        out_data += "\r\n";
        // 4. 请求正文（如果有）
        if (!body_.empty())
        {
            out_data += body_;
        }
        return true;
    }

    // 解析原始 HTTP 请求数据
    bool deserialize(const std::string &raw_data)
    {
        // TODO: 解析请求行、请求报头、空行、请求正文

        // 1. 先把 raw_data 按 \r\n 拆成多行
        std::vector<std::string> lines;
        size_t start = 0;
        size_t pos = raw_data.find(Sep); // Sep = "\r\n"
        while (pos != std::string::npos)
        {
            lines.push_back(raw_data.substr(start, pos - start));
            start = pos + Sep.size();
            pos = raw_data.find(Sep, start);
        }
        lines.push_back(raw_data.substr(start)); // 最后一段，不一定是整个正文，可能是正文的一部分，因为正文可能也有分隔符已经分割放进lines了

        // 2. 解析请求行（第一行）
        if (lines.empty())
            return false;
        std::vector<std::string> parts = split(lines[0], ' '); // 按空格拆
        if (parts.size() != 3)
            return false;
        method_ = parts[0];
        uri_ = parts[1];
        version_ = parts[2];

        // 3. 解析请求报头（直到遇到空行）
        size_t i = 1;
        while (i < lines.size() && !lines[i].empty())
        {
            size_t colon = lines[i].find(':');
            if (colon != std::string::npos)
            {
                std::string key = lines[i].substr(0, colon);
                std::string value = lines[i].substr(colon + 1);
                // 去掉前导空格，协议是允许有空格，所以可能没有可能多个
                // 注：协议其实也支持 '\t' ，但几乎不使用，忽略
                while (!value.empty() && value[0] == ' ')
                {
                    value.erase(0, 1);
                }
                headers_[key] = value;
            }
            i++;
        }

        // 4. 跳过空行
        if (i < lines.size() && lines[i].empty())
            i++;

        // 5. 剩余部分就是正文（可选：根据 Content-Length 截取）
        body_.clear();
        for (size_t j = i; j < lines.size(); ++j)
        {
            if (!body_.empty())
                body_ += "\r\n";
            body_ += lines[j];
        }

        return true;
    }

    // 构建请求报文
    std::string build_request_str()
    {
        std::string msg;
        // 1. 请求行
        msg += method_ + " " + uri_ + " " + version_ + Sep;
        // 2. 请求头
        for (auto &kv : headers_)
        {
            msg += kv.first + ": " + kv.second + Sep;
        }
        // 3. 空行
        msg += Sep;
        // 4. 正文
        msg += body_;
        return msg;
    }

    // 设置字段
    void set_method(const std::string &m) { method_ = m; }
    void set_uri(const std::string &u) { uri_ = u; }
    void set_version(const std::string &v) { version_ = v; }
    void set_header(const std::string &key, const std::string &value)
    {
        headers_[key] = value;
    }
    void set_body(const std::string &b) { body_ = b; }

    // 获取请求方法
    std::string get_method() const { return method_; }
    // 获取 URI
    std::string get_uri() const { return uri_; }
    // 获取 HTTP 版本
    std::string get_version() const { return version_; }
    // 获取请求报头
    std::string get_header(const std::string &key) const
    {
        auto it = headers_.find(key);
        if (it != headers_.end())
        {
            return it->second;
        }
        return "";
    }
    // 获取请求正文
    std::string get_body() const { return body_; }

    // 清空所有数据
    void clear()
    {
        method_.clear();
        uri_.clear();
        version_.clear();
        headers_.clear();
        body_.clear();
    }

private:
    // 请求行
    std::string method_;  // GET / POST / PUT / DELETE ...
    std::string uri_;     // /index.html /api/user ...
    std::string version_; // HTTP/1.1

    // 请求报头
    std::unordered_map<std::string, std::string> headers_; // key -> value

    // 请求正文
    std::string body_; // POST 提交的数据
};

// 状态行 - 【HTTP版本】【空格】【状态码】【空格】【状态码描述】【换行符】
// 响应报头 - 【Key】【:】【空格】【Value】
// 响应报头 - ...
// 空行 - 【换行符】
// 响应正文 - 【DATA】
class Response
{
public:
    // 设置版本

    void set_version(const std::string &version)
    {
        version_ = version;
    }
    // 设置状态码（自动填充状态码描述）
    void set_status(int code)
    {
        status_code_ = code;
        status_desc_ = get_status_desc(code);
    }

    // 设置单个报头
    void set_header(const std::string &key, const std::string &value)
    {
        headers_[key] = value;
    }

    // 设置正文（会自动计算 Content-Length）
    void set_body(const std::string &body)
    {
        body_ = body;
        headers_["Content-Length"] = std::to_string(body.size());
    }

    // 序列化为 HTTP 响应字符串
    std::string serialize() const
    {
        std::string result;

        // 1. 状态行
        result += "HTTP/1.1 " + std::to_string(status_code_) + " " + status_desc_ + "\r\n";

        // 2. 响应报头
        for (const auto &[key, value] : headers_)
        {
            result += key + ": " + value + "\r\n";
        }

        // 3. 空行
        result += "\r\n";

        // 4. 正文（如果有）
        if (!body_.empty())
        {
            result += body_;
        }

        return result;
    }

private:
    // 状态码 → 描述 转换函数
    std::string get_status_desc(int code) const
    {
        switch (code)
        {
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 400:
            return "Bad Request";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 500:
            return "Internal Server Error";
        default:
            return "Unknown";
        }
    }

private:
    std::string version_;
    int status_code_ = 200;
    std::string status_desc_ = "OK";
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
};