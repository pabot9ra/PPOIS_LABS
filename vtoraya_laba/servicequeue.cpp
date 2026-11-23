#include "servicequeue.h"
#include "workinghours.h"
#include <limits>
#include <stdexcept>

ServiceQueue::ServiceQueue(int id, const PostalBranch &branch)
    : id_(id), branchId_(branch.id()), nextNumber_(1), issuingOpen_(true) {
    if(id_ <= 0){
        throw std::invalid_argument("Queue ID must be positive");
    }
}

int ServiceQueue::id() const {
    return id_;
}

int ServiceQueue::branchId() const {
    return branchId_;
}

bool ServiceQueue::issuingOpen() const {
    return issuingOpen_;
}

std::size_t ServiceQueue::waitingCount() const {
    std::size_t count = 0;
    for(const QueueTicket &ticket : tickets_){
        if(ticket.status() == TicketStatus::Waiting){
            count++;
        }
    }
    return count;
}

int ServiceQueue::issue(const Customer &customer, const std::string &service, int minute) {
    if(!issuingOpen_){
        throw std::logic_error("Issuing new tickets is closed");
    }
    if(!tickets_.empty() && minute < tickets_.back().issuedMinute()){
        throw std::invalid_argument("Tickets must be issued in chronological order");
    }
    if(nextNumber_ == std::numeric_limits<int>::max()){
        throw std::overflow_error("Queue ticket numbers are exhausted");
    }
    QueueTicket ticket(nextNumber_, customer, service, minute);
    tickets_.push_back(ticket);
    return nextNumber_++;
}

const QueueTicket &ServiceQueue::findTicket(int number) const {
    for(const QueueTicket &ticket : tickets_){
        if(ticket.number() == number){
            return ticket;
        }
    }
    throw std::out_of_range("Queue ticket was not found");
}

QueueTicket &ServiceQueue::findMutableTicket(int number) {
    for(QueueTicket &ticket : tickets_){
        if(ticket.number() == number){
            return ticket;
        }
    }
    throw std::out_of_range("Queue ticket was not found");
}

int ServiceQueue::callNext(int counterId, int minute) {
    if(counterId <= 0 || minute < 0 || minute >= WorkingHours::MINUTES_PER_DAY){
        throw std::invalid_argument("Counter ID and service minute must be valid");
    }
    for(const QueueTicket &ticket : tickets_){
        if(ticket.status() == TicketStatus::Called && ticket.counterId() == counterId){
            throw std::logic_error("Counter already has a called ticket");
        }
    }
    for(QueueTicket &ticket : tickets_){
        if(ticket.status() == TicketStatus::Waiting && ticket.issuedMinute() <= minute){
            ticket.call(counterId);
            return ticket.number();
        }
    }
    throw std::out_of_range("No waiting ticket is available at this time");
}

void ServiceQueue::completeTicket(int number, int counterId) {
    QueueTicket &ticket = findMutableTicket(number);
    if(counterId <= 0 || ticket.counterId() != counterId){
        throw std::invalid_argument("Ticket belongs to another counter");
    }
    ticket.complete();
}

void ServiceQueue::cancelTicket(int number) {
    QueueTicket &ticket = findMutableTicket(number);
    ticket.cancel();
}

void ServiceQueue::closeIssuing() {
    if(!issuingOpen_){
        throw std::logic_error("Ticket issuing is already closed");
    }
    issuingOpen_ = false;
}

void ServiceQueue::openIssuing() {
    if(issuingOpen_){
        throw std::logic_error("Ticket issuing is already open");
    }
    issuingOpen_ = true;
}
