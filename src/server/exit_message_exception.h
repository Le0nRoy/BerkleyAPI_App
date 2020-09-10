//
// Created by lap on 9/9/20.
//

#ifndef SOCKETSERVER_EXIT_MESSAGE_EXCEPTION_H
#define SOCKETSERVER_EXIT_MESSAGE_EXCEPTION_H


#include <string>

class ExitMessageException : public std::exception
{
    std::string message;

public:
    ExitMessageException() = default;
    explicit ExitMessageException(char* message) :
            message(message)
    {}

    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override
    {
        return message.data();
    }
};


#endif //SOCKETSERVER_EXIT_MESSAGE_EXCEPTION_H
