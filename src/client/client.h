//
// Created by lap on 9/5/20.
//

#ifndef SOCKETSERVER_CLIENT_H
#define SOCKETSERVER_CLIENT_H

#include <utility>

#include "../socket.h"

class Client : public Socket
{
public:
    explicit Client(std::string ip, int port = 11000,
                    connectionType conType = TCP) :
            Socket(),
            ip(std::move(ip))
    {
        this->port = port;
        this->conType = conType;
    }

    void run();

private:
    std::string ip;

    int createConnection() override;

};


#endif //SOCKETSERVER_CLIENT_H
