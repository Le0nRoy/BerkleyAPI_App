//
// Created by lap on 9/9/20.
//

#ifndef SOCKETSERVER_LOGGER_H
#define SOCKETSERVER_LOGGER_H

//#include "spdlog/spdlog.h"

class Logger
{
    static Logger *instance;

    Logger() = default;

public:
    static Logger *getInstance();

    void info(const std::string &message);

    void error(const std::string &message);
};


#endif //SOCKETSERVER_LOGGER_H
