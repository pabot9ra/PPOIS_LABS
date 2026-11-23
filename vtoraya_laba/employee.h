#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "postalbranch.h"
#include <string>

class Employee {
private:
    int id_;
    std::string name_;
    int branchId_;
    bool onShift_;

protected:
    void ensureCanWork(const PostalBranch &branch) const;

public:
    Employee(int id, const std::string &name, int branchId);
    virtual ~Employee() = default;
    Employee &operator=(const Employee &other) = delete;

    int id() const;
    const std::string &name() const;
    int branchId() const;
    bool onShift() const;
    virtual std::string role() const = 0;

    virtual void startShift();
    virtual void endShift();
    virtual void assignToBranch(int branchId);
};

#endif
