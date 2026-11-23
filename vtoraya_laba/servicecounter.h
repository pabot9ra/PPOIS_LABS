#ifndef SERVICECOUNTER_H
#define SERVICECOUNTER_H

#include "workinghours.h"
#include "servicequeue.h"
#include "employee.h"

class ServiceCounter {
private:
    int id_;
    int branchId_;
    WorkingHours hours_;
    bool open_;
    int employeeId_;
    int currentTicket_;
    int currentQueueId_;

    void ensureStaff(const Employee &employee) const;
    void finishTicket(ServiceQueue &queue);

public:
    ServiceCounter(int id, const PostalBranch &branch, const WorkingHours &hours);

    int id() const;
    int branchId() const;
    const WorkingHours &hours() const;
    bool isOpen() const;
    int employeeId() const;
    int currentTicket() const;
    void open(const Employee &employee, int minute);
    int callNext(ServiceQueue &queue, const Employee &employee, int minute, int expectedMinutes);
    void finishCurrent(ServiceQueue &queue, const Employee &employee);
    void close(const Employee &employee);
};

#endif
