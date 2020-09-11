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
#include <libnet.h>

void Server::startServer()
{
    if (createConnection() == EXIT_FAILURE)
    {
        return;
    }

    bool hasConnected = false;
    bool serverRunning = true;
    int maxFd;
    int i;
    int connectFd;
    unsigned long long res;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    fd_set readfds;

    while (serverRunning)
    {
        FD_ZERO(&readfds);
        FD_SET(serverFd, &readfds);

        res = 0;
        maxFd = serverFd;

        for (i = 0; i < maxClients; ++i)
        {
            connectFd = clientFds[i];
            if (connectFd > 0)
            {
                FD_SET(connectFd, &readfds);
            }
            if (connectFd > maxFd)
            {
                maxFd = connectFd;
            }
        }

        res = select(maxFd + 1, &readfds, nullptr, nullptr, nullptr);

        if ((res < 0) && (errno != EINTR))
        {
            logger->error("Failed to select socket");
        }

        if (FD_ISSET(serverFd, &readfds))
        {
            connectFd = accept(serverFd, (struct sockaddr *)&cliaddr, &cliaddrlen);
            if (connectFd < 0)
            {
                logger->error("Failed to accept connection");
                logger->error(strerror(errno));
//                closeConnection(serverFd);
                break;
            }
            else
            {
                for (i = 0; i < maxClients; ++i)
                {
                    if (clientFds[i] == 0)
                    {
                        clientFds[i] = connectFd;
                        hasConnected = true;
                        logger->info("Accepted connection from client");
                        logger->info("ip: " + std::string(inet_ntoa(cliaddr.sin_addr)));
                        logger->info("port: " + std::to_string(ntohs(cliaddr.sin_port)));
                        break;
                    }
                }
                if (!hasConnected)
                {
                    // TODO log if couldn't add to connections
                    logger->error("Failed to ");
                }
            }
        }

        for (i = 0; i < maxClients; ++i)
        {
            connectFd = clientFds[i];
            if (FD_ISSET(connectFd, &readfds))
            {
                getMessage(connectFd);
                try
                {
                    res = parseMessage();
                }
                catch (NoNumbersException &e)
                {
                    sendMessage(connectFd, buffer);
                    continue;
                }
                catch (ExitMessageException &e)
                {
                    closeConnection(connectFd);
                    clientFds[i] = 0;
                    logger->info("Client has disconnected");
                    // FIXME need map with addresses and FDs
//                    logger->info("ip: " + std::string(inet_ntoa(cliaddr.sin_addr)));
//                    logger->info("port: " + std::to_string(ntohs(cliaddr.sin_port)));
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
        }
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

    if (str.empty() || str == "exit")
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
    if (listen(serverFd, backlog) == -1)
    {
        logger->error("Error while listening to socket.");
        logger->error(strerror(errno));
        close(serverFd);
        return EXIT_FAILURE;
    }

    logger->info("Server started successfully.");

    return EXIT_SUCCESS;
}

Server::~Server()
{
    for (int & clientFd : clientFds)
    {
        if (clientFd != 0)
        {
            closeConnection(clientFd);
        }
    }
}
