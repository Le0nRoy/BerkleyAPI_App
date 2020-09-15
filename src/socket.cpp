//
// Created by lap on 9/9/20.
//

#include "socket.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int Socket::sendMessage(const int &fd, const std::string &message)
{
    if (conType == TCP)
    {
        return write(fd, message.data(), message.size());
    }
    else if (conType == UDP)
    {
        return sendto(serverFd, buffer, strlen(buffer), 0,
                      (struct sockaddr *)&sa, sizeof sa);
    }
}

int Socket::getMessage(const int &fd)
{
    ssize_t recsize = 0;

    if (conType == TCP)
    {
        recsize = read(fd, buffer, buflen);
    }
    else if (conType == UDP)
    {
        socklen_t fromlen = sizeof sa;

        recsize = read(fd, buffer, buflen);
//        recsize = recvfrom(serverFd, (void *)buffer, sizeof buffer, 0,
//                           (struct sockaddr *)&sa, &fromlen);
    }

    if (recsize < 0)
    {
        return recsize;
    }
    buffer[recsize] = '\0';
    return 0;
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
