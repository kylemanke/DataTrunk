#ifndef DT_SRC_EXCEPTION_TOKEN_EXCEPTION_H
#define DT_SRC_EXCEPTION_TOKEN_EXCEPTION_H

#include <exception>
#include <string>

class TokenException : public std::exception {
public:
    explicit TokenException(const char* msg) : msg_(msg) {}
    explicit TokenException(const std::string& msg) : msg_(msg) {}

    virtual ~TokenException() noexcept {}

    virtual const char* what() const noexcept {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

#endif