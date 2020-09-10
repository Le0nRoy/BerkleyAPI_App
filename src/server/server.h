//
// Created by lap on 9/5/20.
//

#ifndef SOCKETSERVER_SERVER_H
#define SOCKETSERVER_SERVER_H

#include "../socket.h"

#include <string>

class Server : public Socket
{
public:
    explicit Server(int port = 11000, connectionType conType = TCP)
    {
        this->port = port;
        this->conType = conType;
    }

    void startServer();

private:
    int backlog = 10;
protected:
    int createConnection() override;

private:
    unsigned long long parseMessage();
};


#endif //SOCKETSERVER_SERVER_H
