#ifndef INVALIDCUSTOMEREXCEPTION_H
#define INVALIDCUSTOMEREXCEPTION_H

#include "postalexception.h"

class InvalidCustomerException : public PostalException {
private:
    int customerId_;

public:
    InvalidCustomerException(int customerId, const std::string &message);

    int customerId() const;
};

#endif
