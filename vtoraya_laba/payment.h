#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

class Payment {
private:
    int id_;
    std::string trackingNumber_;
    int amount_;
    bool captured_;
    int refundedAmount_;

public:
    Payment(int id, const std::string &trackingNumber, int amount);

    int id() const;
    const std::string &trackingNumber() const;
    int amount() const;
    bool captured() const;
    int refundableAmount() const;
    void capture();
    void refund(int amount);
};

#endif
