#ifndef POSTALEXCEPTION_H
#define POSTALEXCEPTION_H

#include <stdexcept>
#include <string>

class PostalException : public std::runtime_error {
public:
    explicit PostalException(const std::string &message);
};

#endif
