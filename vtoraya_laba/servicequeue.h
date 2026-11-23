#ifndef SERVICEQUEUE_H
#define SERVICEQUEUE_H

#include "queueticket.h"
#include "postalbranch.h"
#include <cstddef>
#include <vector>

class ServiceQueue {
private:
    int id_;
    int branchId_;
    int nextNumber_;
    std::vector<QueueTicket> tickets_;
    bool issuingOpen_;

    QueueTicket &findMutableTicket(int number);

public:
    ServiceQueue(int id, const PostalBranch &branch);

    int id() const;
    int branchId() const;
    bool issuingOpen() const;
    std::size_t waitingCount() const;
    int issue(const Customer &customer, const std::string &service, int minute);
    const QueueTicket &findTicket(int number) const;
    int callNext(int counterId, int minute);
    void completeTicket(int number, int counterId);
    void cancelTicket(int number);
    void closeIssuing();
    void openIssuing();
};

#endif
