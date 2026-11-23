#include "servicecounter.h"
#include <stdexcept>

ServiceCounter::ServiceCounter(int id, const PostalBranch &branch, const WorkingHours &hours)
    : id_(id), branchId_(branch.id()), hours_(hours), open_(false), employeeId_(0),
      currentTicket_(0), currentQueueId_(0) {
    if(id_ <= 0){
        throw std::invalid_argument("Service counter ID must be positive");
    }
}

int ServiceCounter::id() const {
    return id_;
}

int ServiceCounter::branchId() const {
    return branchId_;
}

const WorkingHours &ServiceCounter::hours() const {
    return hours_;
}

bool ServiceCounter::isOpen() const {
    return open_;
}

int ServiceCounter::employeeId() const {
    return employeeId_;
}

int ServiceCounter::currentTicket() const {
    return currentTicket_;
}

void ServiceCounter::ensureStaff(const Employee &employee) const {
    if(!open_ || !employee.onShift() || employee.id() != employeeId_ || employee.branchId() != branchId_){
        throw std::logic_error("Counter must be open with its assigned employee on shift");
    }
}

void ServiceCounter::open(const Employee &employee, int minute) {
    if(open_ || !employee.onShift() || employee.branchId() != branchId_){
        throw std::logic_error("Counter requires a free window and an employee on shift at this branch");
    }
    if(!hours_.isOpenAt(minute)){
        throw std::logic_error("Counter is outside its working hours");
    }
    employeeId_ = employee.id();
    open_ = true;
}

int ServiceCounter::callNext(ServiceQueue &queue, const Employee &employee, int minute, int expectedMinutes) {
    ensureStaff(employee);
    if(currentTicket_ != 0){
        throw std::logic_error("Finish the current ticket before calling another one");
    }
    if(queue.branchId() != branchId_){
        throw std::invalid_argument("Queue belongs to another branch");
    }
    if(!hours_.canServe(minute, expectedMinutes)){
        throw std::logic_error("Service must fit within working time without crossing a break");
    }
    currentTicket_ = queue.callNext(id_, minute);
    currentQueueId_ = queue.id();
    return currentTicket_;
}

void ServiceCounter::finishCurrent(ServiceQueue &queue, const Employee &employee) {
    ensureStaff(employee);
    if(currentTicket_ == 0){
        throw std::logic_error("Counter has no current ticket");
    }
    if(queue.branchId() != branchId_ || queue.id() != currentQueueId_){
        throw std::invalid_argument("Ticket was called from another queue");
    }
    finishTicket(queue);
    currentTicket_ = 0;
    currentQueueId_ = 0;
}

void ServiceCounter::finishTicket(ServiceQueue &queue) {
    const QueueTicket &ticket = queue.findTicket(currentTicket_);
    if(ticket.counterId() != id_){
        throw std::invalid_argument("Ticket belongs to another counter");
    }
    if(ticket.status() == TicketStatus::Called){
        queue.completeTicket(currentTicket_, id_);
    } else if(ticket.status() != TicketStatus::Served){
        throw std::logic_error("Current ticket cannot be completed");
    }
}

void ServiceCounter::close(const Employee &employee) {
    ensureStaff(employee);
    if(currentTicket_ != 0){
        throw std::logic_error("Finish the current ticket before closing the counter");
    }
    open_ = false;
    employeeId_ = 0;
}
