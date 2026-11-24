#ifndef PAYMENTNOTFOUNDEXCEPTION_H
#define PAYMENTNOTFOUNDEXCEPTION_H

#include "postalexception.h"

class PaymentNotFoundException : public PostalException {
private:
    int paymentId_;

public:
    explicit PaymentNotFoundException(int paymentId);

    int paymentId() const;
};

#endif
