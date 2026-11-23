#ifndef RECEIPT_H
#define RECEIPT_H

#include "payment.h"
#include <string>

class Receipt {
private:
    int registerId_;
    int paymentId_;
    std::string trackingNumber_;
    int amount_;

public:
    Receipt(int registerId, const Payment &payment);

    int registerId() const;
    int paymentId() const;
    const std::string &trackingNumber() const;
    int amount() const;
    std::string format() const;
};

#endif
