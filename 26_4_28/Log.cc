// 日志源文件

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include <filesystem>
#include <memory>
#include <sstream>
#include <fstream>
#include "Mutex.hpp"
#include "Log.hpp"

namespace log_module
{
    // 默认路径和⽇志名称
    const std::string defaultpath = "./log/";
    const std::string defaultname = "log.txt";

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
    Logger::LogMessage::LogMessage(const LogLevel &level, const std::string &filename, int line, Logger &logger)
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

    std::string Logger::LogMessage::gettime()
    {
        time_t t = time(nullptr);
        struct tm local;
        localtime_r(&t, &local);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local);
        return buf;
    }

    std::string Logger::LogMessage::level_tostr()
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

    Logger::LogMessage::~LogMessage()
    {
        _logger._strategy.get()->SyncLog(_loginfo);
    }

    Logger::Logger()
    {
        _strategy = std::make_shared<ConsoleLogStrategy>();
    }

    void Logger::EnableConsoleLog()
    {
        _strategy = std::make_shared<ConsoleLogStrategy>();
    }

    void Logger::EnableFileLog()
    {
        _strategy = std::make_shared<FileLogStrategy>();
    }

    Logger::LogMessage Logger::operator()(const LogLevel &level, const std::string &filename, int line)
    {
        return Logger::LogMessage(level, filename, line, *this);
    }

    Logger &GetLogger()
    {
        static Logger log;
        return log;
    }
}
