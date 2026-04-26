#include "Log.hpp"
#include <unistd.h>

using namespace log_module;

int main()
{
    ENABLE_FILE_LOG_STRATEGY();

    int i = 1;
    while (i <= 10)
    {
        //log_module::log(log_module::LogLevel::DEBUG, __FILE__, __LINE__) << "测试" << i++;
        LOG(LogLevel::DEBUG) << "测试" << i++;
        sleep(1);
    }
    
    return 0;
}