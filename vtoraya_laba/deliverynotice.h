#ifndef DELIVERYNOTICE_H
#define DELIVERYNOTICE_H

#include "postalbranch.h"
#include <string>

class DeliveryNotice {
private:
    int id_;
    std::string trackingNumber_;
    std::string recipientName_;
    std::string contact_;
    int branchId_;
    std::string branchAddress_;
    bool sent_;
    bool read_;

public:
    DeliveryNotice(int id, const PostalBranch &branch, const std::string &trackingNumber,
                   const std::string &contact);

    int id() const;
    const std::string &trackingNumber() const;
    const std::string &contact() const;
    int branchId() const;
    bool sent() const;
    bool read() const;
    std::string formatMessage() const;
    void markSent();
    void markRead();
};

#endif
