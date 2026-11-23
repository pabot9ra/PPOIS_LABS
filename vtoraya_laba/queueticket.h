#ifndef QUEUETICKET_H
#define QUEUETICKET_H

#include "customer.h"
#include <string>

enum class TicketStatus {
    Waiting,
    Called,
    Served,
    Cancelled
};

class QueueTicket {
private:
    int number_;
    Customer customer_;
    std::string service_;
    int issuedMinute_;
    TicketStatus status_;
    int counterId_;

public:
    QueueTicket(int number, const Customer &customer, const std::string &service, int issuedMinute);

    int number() const;
    const Customer &customer() const;
    const std::string &service() const;
    int issuedMinute() const;
    TicketStatus status() const;
    int counterId() const;
    void call(int counterId);
    void complete();
    void cancel();
};

#endif
