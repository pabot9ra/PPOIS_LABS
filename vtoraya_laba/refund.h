#ifndef REFUND_H
#define REFUND_H

#include <string>

class Refund {
private:
    int id_;
    int paymentId_;
    int amount_;
    std::string reason_;

public:
    Refund(int id, int paymentId, int amount, const std::string &reason);

    int id() const;
    int paymentId() const;
    int amount() const;
    const std::string &reason() const;
};

#endif
