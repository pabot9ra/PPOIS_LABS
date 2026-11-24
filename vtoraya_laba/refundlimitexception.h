#ifndef REFUNDLIMITEXCEPTION_H
#define REFUNDLIMITEXCEPTION_H

#include "postalexception.h"

class RefundLimitException : public PostalException {
private:
    int requestedAmount_;
    int availableAmount_;

public:
    RefundLimitException(int requestedAmount, int availableAmount);

    int requestedAmount() const;
    int availableAmount() const;
};

#endif
