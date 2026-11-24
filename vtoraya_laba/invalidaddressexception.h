#ifndef INVALIDADDRESSEXCEPTION_H
#define INVALIDADDRESSEXCEPTION_H

#include "postalexception.h"

class InvalidAddressException : public PostalException {
private:
    std::string field_;

public:
    InvalidAddressException(const std::string &field, const std::string &message);

    const std::string &field() const;
};

#endif
