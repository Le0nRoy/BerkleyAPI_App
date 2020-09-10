#include "src/client/client.h"
#include "src/server/server.h"

#include <string>
#include <iostream>

// TODO Task:
//  1. Client sends message from console to server                      OK
//  2. Server receives that message and parses it:
//  2.1. Finds all numbers in it
//  2.2 Counts sum of all numbers
//  3. Sends sum back to client
//  3.1. If no numbers in client message,
//  server sends message back to client
//  4. Client and server should support next protocols:
//  4.1. TCP
//  4.2. UDP
//  5. Server should correctly serve multiple clients on both protocols
//  6. Client should be able to chose protocol on start
//  7. Client should be able to send multiple messages
//  without restart or reconnection

// TODO Debug client
//  1. Connect to server                        OK
//  2. Send message with numbers
//  2.1. Send message with 0
//  2.2. Send message with words and numbers
//  3. Send message without numbers
//  3.1. Send empty message
//  4. Send "exit"

// TODO Debug server
//  1. Start server                             OK
//  2. Get connection from client               OK
//  3. Get multiple messages from client
//  4. Close connection with client
//  5. Get new connection from client
//  Optional: Write messages in logs

#define USAGE \
    std::string appName(argc[0]); \
    int pos = appName.find_last_of('/'); \
    appName = appName.substr(pos + 1);   \
    std::cout << "Usage: " << std::endl  \
    << appName << " server <port> <protocol>" << std::endl \
    << "or: " << std::endl        \
    << appName << " client <port> <protocol> <ip>" << std::endl \
    << "protocol should be TCP or UDP" << std::endl; \
    return EXIT_FAILURE;

int main(int argv, char **argc)
{
    if (argv < 4)
    {
        USAGE
    }

    std::string appMode(argc[1]);
    int port;
    try
    {
        std::string portStr(argc[2]);
        port = std::stoi(portStr);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        USAGE
    }
    std::string protocolStr(argc[3]);
    connectionType con;
    if (protocolStr == "TCP")
    {
        con = TCP;
    }
    else if (protocolStr == "UDP")
    {
        con = UDP;
        return 1;
    }
    else
    {
        USAGE
    }

    if (appMode == "server")
    {
        Server server(port, con);
        server.startServer();
    }
    else if (appMode == "client" && argv == 5)
    {
        std::string ip(argc[4]);
        Client client(ip, port, con);
        client.run();
    }
    else
    {
        USAGE
    }

    return EXIT_SUCCESS;
}
