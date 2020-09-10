//
// Created by lap on 9/5/20.
//

#include "server.h"
#include "no_numbers_exception.h"
#include "exit_message_exception.h"

#include <cstring>
#include <string>

#include <netinet/in.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/select.h>

void Server::startServer()
{
    if (createConnection() == EXIT_FAILURE)
    {
        return;
    }

//    int fd1, fd2;
//    fd_set rfds;
//    struct timeval timeout;
//    timeout.tv_sec = 1;   // 1 second timeout
//    timeout.tv_usec = 0;
//    FD_ZERO(&rfds);
//    FD_SET(fd1, &rfds);
//    FD_SET(fd2, &rfds);
//    int rc = select(max(fd1, fd2) + 1, &rfds, NULL, NULL, &timeout);
//    if (rc == 0) // timeout
//    if (rc > 0)
//    {
//        if (FD_ISSET(fd1, &rfds))
//        {
//            // Data available to read on fd1
//        }
//        if (FD_ISSET(fd2, &rfds))
//        {
//            // Data available to read on fd2
//        }
//    }
//    if (rc < 0) // error

    bool serverRunning = true;
    while (serverRunning)
    {
        // FIXME here will be problems
        // TODO listen to multiple sockets
        unsigned long long res = 0;
        // TODO listen to connection
        struct sockaddr_in cliaddr;
        socklen_t cliaddrlen = sizeof(cliaddr);
        int connectFd = accept(serverFd, (struct sockaddr *) &cliaddr, &cliaddrlen);

        if (connectFd < 0)
        {
            logger->error("Failed to accept connection");
            logger->error(strerror(errno));
            closeConnection(serverFd);
            break;
        }
        else
        {
            logger->info("Accepted connection from client");
            // TODO add ip:port
        }

        // TODO choose correct socket
        getMessage(connectFd);
        try
        {
            res = parseMessage();
        }
        catch (NoNumbersException &e)
        {
            sendMessage(connectFd, buffer);
        }
        catch (ExitMessageException &e)
        {
            closeConnection(connectFd);
            continue;
        }
        catch (std::exception &e)
        {
            logger->error("Unexpected exception occurred.");
            logger->error(e.what());
            serverRunning = false;
            break;
        }

        sendMessage(connectFd, std::to_string(res));
    }

    closeConnection(serverFd);
    logger->info("Server has closed.");
}

/// Good
unsigned long long Server::parseMessage()
{
    unsigned long long res = 0;
    unsigned long long num = 0;
    bool noNumbers = true;
    std::string str(buffer);

    if (str.empty())
    {
        return -1;
    }

    if (str == "exit")
    {
        throw ExitMessageException();
    }

    for (auto c : str)
    {
        if (c >= '0' && c <= '9')
        {
            noNumbers = false;
            num *= 10;
            num += c - '0';
        }
        else
        {
            res += num;
            num = 0;
        }
    }
    res += num;
    num = 0;

    if (noNumbers)
    {
        throw NoNumbersException();
    }
    return res;
}

/// Good
int Server::createConnection()
{
    int res;
    struct sockaddr_in sa;

    res = Socket::createConnection();
    if (res == EXIT_FAILURE)
    {
        logger->error("Failed to create socket.");
        logger->error(strerror(errno));
        return EXIT_FAILURE;
    }

    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverFd, (struct sockaddr *)&sa, sizeof sa) == -1)
    {
        logger->error("Error binding socket to address.");
        logger->error(strerror(errno));
        close(serverFd);
        return EXIT_FAILURE;
    }
    if (listen(serverFd, backlog) == -1) {
        logger->error("Error while listening to socket.");
        logger->error(strerror(errno));
        close(serverFd);
        return EXIT_FAILURE;
    }

    logger->info("Server started successfully.");

    return EXIT_SUCCESS;
}
