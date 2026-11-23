#include "queueticket.h"
#include "workinghours.h"
#include <cctype>
#include <stdexcept>

QueueTicket::QueueTicket(int number, const Customer &customer, const std::string &service, int issuedMinute)
    : number_(number), customer_(customer), service_(service), issuedMinute_(issuedMinute),
      status_(TicketStatus::Waiting), counterId_(0) {
    if(number_ <= 0 || issuedMinute_ < 0 || issuedMinute_ >= WorkingHours::MINUTES_PER_DAY){
        throw std::invalid_argument("Ticket requires positive number and a valid minute of day");
    }
    bool hasService = false;
    for(char character : service_){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasService = true;
            break;
        }
    }
    if(!hasService){
        throw std::invalid_argument("Service name must not be blank");
    }
}

int QueueTicket::number() const {
    return number_;
}

const Customer &QueueTicket::customer() const {
    return customer_;
}

const std::string &QueueTicket::service() const {
    return service_;
}

int QueueTicket::issuedMinute() const {
    return issuedMinute_;
}

TicketStatus QueueTicket::status() const {
    return status_;
}

int QueueTicket::counterId() const {
    return counterId_;
}

void QueueTicket::call(int counterId) {
    if(counterId <= 0){
        throw std::invalid_argument("Counter ID must be positive");
    }
    if(status_ != TicketStatus::Waiting){
        throw std::logic_error("Only a waiting ticket can be called");
    }
    counterId_ = counterId;
    status_ = TicketStatus::Called;
}

void QueueTicket::complete() {
    if(status_ != TicketStatus::Called){
        throw std::logic_error("Only a called ticket can be completed");
    }
    status_ = TicketStatus::Served;
}

void QueueTicket::cancel() {
    if(status_ != TicketStatus::Waiting){
        throw std::logic_error("Only a waiting ticket can be cancelled");
    }
    status_ = TicketStatus::Cancelled;
}
