//
// Created by lap on 9/5/20.
//

#ifndef SOCKETSERVER_NO_NUMBERS_EXCEPTION_H
#define SOCKETSERVER_NO_NUMBERS_EXCEPTION_H


#include <string>
#include <utility>

class NoNumbersException : public std::exception
{
    std::string message;

public:
    NoNumbersException() = default;
    explicit NoNumbersException(char* message) :
        message(message)
    {}

    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override
    {
        return message.data();
    }
};


#endif //SOCKETSERVER_NO_NUMBERS_EXCEPTION_H
