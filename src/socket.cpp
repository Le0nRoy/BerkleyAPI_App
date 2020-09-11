//
// Created by lap on 9/9/20.
//

#include "socket.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int Socket::sendMessage(const int &fd, const std::string &message)
{
    return write(fd, message.data(), message.size());
}

int Socket::getMessage(const int &fd)
{
    ssize_t rcount = read(fd, buffer, buflen);
    if (rcount == -1)
    {
        return rcount;
    }
    buffer[rcount] = '\0';
    return 0;
}

int Socket::closeConnection(int &fd)
{
    if (shutdown(fd, SHUT_RDWR) == -1)
    {
        logger->error("Failed to shutdown connection");
    }
    close(fd);
    fd = 0;
    return EXIT_SUCCESS;
}

Socket::~Socket()
{
    if (serverFd > 0)
    {
        closeConnection(serverFd);
    }
}

int Socket::createConnection()
{
    serverFd = socket(AF_INET, conType, 0);
    if (serverFd == -1)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
