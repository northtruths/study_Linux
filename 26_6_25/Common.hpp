#pragma once

#include <string>
#include <vector>

uint16_t gdefaultport = -1;
int gdefaultfd = -1;
int glisten_num = 10;

std::string root = "./myhttp";

std::string Sep = "\r\n";

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t pos = s.find(delim);
    while (pos != std::string::npos)
    {
        result.push_back(s.substr(start, pos - start));
        start = pos + 1;
        pos = s.find(delim, start);
    }
    result.push_back(s.substr(start));
    return result;
}
