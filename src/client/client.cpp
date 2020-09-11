//
// Created by lap on 9/5/20.
//

#include <iostream>
#include "client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

void Client::run()
{
    std::string str;
    int res;

    if (createConnection() == EXIT_FAILURE)
    {
        return;
    }

    while (true)
    {
        std::cout << ">> ";
        std::getline(std::cin, str);

        if (str == "exit")
        {
            sendMessage(serverFd, str);
            break;
        }
        if (str == "help")
        {
            std::cout << "Type your message to send it on server." << std::endl
                      << "As answer you'll get sum of all numbers in your message." << std::endl
                      << "If no numbers were then you'll get back your message." << std::endl
                      << std::endl
                      << "Some words are reserved as special commands:" << std::endl
                      << "help - show this message" << std::endl
                      << "exit - stop application" << std::endl;
        }

        res = sendMessage(serverFd, str);
        if (res == -1)
        {
            logger->error("Failed to send message");
            logger->error(strerror(errno));
            break;
        }

        res = getMessage(serverFd);
        if (res == -1)
        {
            logger->error("Failed to receive message");
            logger->error(strerror(errno));
            break;
        }

        std::cout << buffer << std::endl;
    }

    logger->info("Closing client...");
}

/// Good
int Client::createConnection()
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
    res = inet_pton(AF_INET, ip.data(), &sa.sin_addr);
    if (res < 0)
    {
        logger->error("Failed to parse IP.");
        logger->error(strerror(errno));
        return EXIT_FAILURE;
    }

    if (connect(serverFd, (sockaddr *)&sa, sizeof sa) == -1)
    {
        logger->error("Failed to create connection with " + ip + ":" +
                      std::to_string(port)
                     );
        logger->error(strerror(errno));
        close(serverFd);
        return EXIT_FAILURE;
    }

    logger->info("Connection established!");
    return EXIT_SUCCESS;
}
