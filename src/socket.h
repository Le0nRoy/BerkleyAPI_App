//
// Created by lap on 9/8/20.
//

#ifndef SOCKETSERVER_SOCKET_H
#define SOCKETSERVER_SOCKET_H

#include <sys/socket.h>
#include <cstdio>
#include <string>
#include <netinet/in.h>
#include "logger/logger.h"

enum connectionType
{
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
};

class Socket
{
public:
    Socket()
    {
        logger = Logger::getInstance();
    }
    virtual ~Socket();
protected:
    Logger *logger;
    static const int buflen = 1024;

    int serverFd;
    int port;
    connectionType conType;
    char buffer[buflen];
    // For UDP
    struct sockaddr_in sa;


    int sendMessage(const int &fd, const std::string &message);
    int getMessage(const int &fd);

    virtual int createConnection();
    int closeConnection(int &fd);

};


#endif //SOCKETSERVER_SOCKET_H
