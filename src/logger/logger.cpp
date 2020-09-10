//
// Created by lap on 9/9/20.
//

#include <string>
#include <iostream>
#include "logger.h"

Logger *Logger::instance = 0;

Logger *Logger::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Logger;
    }
    return instance;
}

void Logger::info(const std::string &message)
{
    std::cout << message << std::endl;
}

void Logger::error(const std::string &message)
{
    std::cerr << message << std::endl;
}


