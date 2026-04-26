// 日志
#pragma once

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include <filesystem>
#include <memory>
#include <sstream>
#include <fstream>
#include "Mutex.hpp"

namespace log_module
{
    // 默认路径和⽇志名称
    const std::string defaultpath = "./log/";
    const std::string defaultname = "log.txt";

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

    // 控制台刷新策略
    class ConsoleLogStrategy : public LogStrategy
    {
    public:
        ConsoleLogStrategy() {}
        void SyncLog(const std::string &message)
        {
            mutex_module::LockGuard lock(_mutex);
            std::cout << message << std::endl;
        }

    private:
        mutex_module::Mutex _mutex;
    };

    // 文件刷新策略
    class FileLogStrategy : public LogStrategy
    {
    public:
        FileLogStrategy(const std::string &logpath = defaultpath, const std::string &logname = defaultname)
        {
            _fullpath = logpath + logname;
            mutex_module::LockGuard lock(_mutex);
            if (std::filesystem::exists(logpath))
            {
                return;
            }
            try
            {
                std::filesystem::create_directories(logpath);
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        void SyncLog(const std::string &message)
        {
            mutex_module::LockGuard lock(_mutex);
            std::ofstream out(_fullpath, std::ios::app);
            out << message << std::endl;
            out.close();
        }

    private:
        mutex_module::Mutex _mutex;
        std::string _fullpath;
    };

    class Logger
    {
    private:
        class LogMessage
        {
        public:
            LogMessage(const LogLevel &level, const std::string &filename, int line, Logger &logger)
                : _curtime(gettime()), _level(level), _pid(getpid()), _filename(filename), _line(line), _logger(logger)
            {
                std::stringstream ss;
                ss << "[" << _curtime << "] "
                   << "[" << level_tostr() << "] "
                   << "[" << std::to_string(_pid) << "] "
                   << "[" << _filename << "] "
                   << "[" << std::to_string(_line) << "] "
                   << "- ";
                _loginfo = ss.str();
            }

            std::string gettime()
            {
                time_t t = time(nullptr);
                struct tm local;
                localtime_r(&t, &local);
                char buf[64];
                strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local);
                return buf;
            }

            std::string level_tostr()
            {
                switch (_level)
                {
                case LogLevel::DEBUG:
                    return "DEBUG";
                case LogLevel::INFO:
                    return "INFO";
                case LogLevel::WARNING:
                    return "WARNING";
                case LogLevel::ERROR:
                    return "ERROR";
                case LogLevel::FATAL:
                    return "FATAL";
                default:
                    return "NONE";
                }
            }

            template <typename T>
            LogMessage &operator<<(const T &info)
            {
                std::stringstream ss;
                ss << info;
                _loginfo += ss.str();
                return *this; //
            }

            ~LogMessage()
            {
                _logger._strategy.get()->SyncLog(_loginfo);
            }

        private:
            std::string _curtime; // 清晰易懂的字符串格式时间
            LogLevel _level;
            pid_t _pid;
            std::string _filename;
            int _line;
            std::string _loginfo; // 一整条日志信息
            Logger &_logger;
        };

    public:
        Logger()
        {
            _strategy = std::make_shared<ConsoleLogStrategy>();
        }

        void EnableConsoleLog()
        {
            _strategy = std::make_shared<ConsoleLogStrategy>();
        }

        void EnableFileLog()
        {
            _strategy = std::make_shared<FileLogStrategy>();
        }

        LogMessage operator()(const LogLevel& level, const std::string& filename, int line){
            return LogMessage(level, filename, line, *this);
        }
    private:
        std::shared_ptr<LogStrategy> _strategy; // 使用的刷新策略
    } log;

#define LOG(loglevel) log(loglevel, __FILE__, __LINE__)

#define ENABLE_CONSOLE_LOG_STRATEGY() log.EnableConsoleLog()
#define ENABLE_FILE_LOG_STRATEGY() log.EnableFileLog()
}