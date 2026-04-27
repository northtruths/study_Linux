// 日志头文件
#pragma once

#include <string>
#include<sstream>
#include<memory>

namespace log_module
{
    // 默认路径和⽇志名称
    extern const std::string defaultpath;
    extern const std::string defaultname;

    // 日志等级
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    // 刷新策略基类
    class LogStrategy
    {
    public:
        virtual ~LogStrategy() = default;
        virtual void SyncLog(const std::string &message) = 0;
    };

    // 日志库类
    class Logger
    {
    public:
        class LogMessage
        {
        public:
            LogMessage(const LogLevel &level, const std::string &filename, int line, Logger &logger);
            std::string gettime();
            std::string level_tostr();
            
            template <typename T>
            LogMessage &operator<<(const T &info)
            {
                std::stringstream ss;
                ss << info;
                _loginfo += ss.str();
                return *this; //
            }
            ~LogMessage();
        private:
            std::string _curtime; // 清晰易懂的字符串格式时间
            LogLevel _level;
            pid_t _pid;
            std::string _filename;
            int _line;
            std::string _loginfo; // 一整条日志信息
            Logger &_logger;
        };

        Logger();
        void EnableConsoleLog();
        void EnableFileLog();
        LogMessage operator()(const LogLevel &level, const std::string &filename, int line);
    private:
        std::shared_ptr<LogStrategy> _strategy; // 使用的刷新策略
    };
    extern Logger log;

#define LOG(loglevel) log(loglevel, __FILE__, __LINE__)
#define ENABLE_CONSOLE_LOG_STRATEGY() log.EnableConsoleLog()
#define ENABLE_FILE_LOG_STRATEGY() log.EnableFileLog()
}