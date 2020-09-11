//
// Created by lap on 9/5/20.
//

#ifndef SOCKETSERVER_SERVER_H
#define SOCKETSERVER_SERVER_H

#include "../socket.h"

#include <string>
#include <cstring>

class Server : public Socket
{
public:
    explicit Server(int port = 11000, connectionType conType = TCP)
    {
        this->port = port;
        this->conType = conType;
        memset(clientFds, 0, sizeof(int) * maxClients);
    }

    void startServer();
    ~Server();

private:
    int backlog = 10;
    static const int maxClients = 10;
    int clientFds[maxClients];

    unsigned long long parseMessage();

protected:
    int createConnection() override;

};


#endif //SOCKETSERVER_SERVER_H
