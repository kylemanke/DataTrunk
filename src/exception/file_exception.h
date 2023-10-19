#ifndef DT_SRC_EXCEPTION_FILE_EXCEPTION_H
#define DT_SRC_EXCEPTION_FILE_EXCEPTION_H

#include <exception>
#include <string>

class FileException : public std::exception {
public:
    explicit FileException(const char* msg) : msg_(msg) {}
    explicit FileException(const std::string& msg) : msg_(msg) {}

    virtual ~FileException() noexcept {}

    virtual const char* what() const noexcept {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

#endif