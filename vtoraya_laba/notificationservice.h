#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include "deliverynotice.h"
#include <cstddef>
#include <vector>

class NotificationService {
private:
    int branchId_;
    int nextId_;
    std::vector<DeliveryNotice> notices_;

    DeliveryNotice &findMutableNotice(int id);

public:
    explicit NotificationService(const PostalBranch &branch);

    int branchId() const;
    std::size_t noticeCount() const;
    int createNotice(const PostalBranch &branch, const std::string &trackingNumber,
                     const std::string &contact);
    const DeliveryNotice &findNotice(int id) const;
    void markSent(int id);
    void markRead(int id);
};

#endif
