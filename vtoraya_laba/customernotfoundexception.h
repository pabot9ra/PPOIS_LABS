#ifndef CUSTOMERNOTFOUNDEXCEPTION_H
#define CUSTOMERNOTFOUNDEXCEPTION_H

#include "postalexception.h"

class CustomerNotFoundException : public PostalException {
private:
    int customerId_;

public:
    explicit CustomerNotFoundException(int customerId);

    int customerId() const;
};

#endif
