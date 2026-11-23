#ifndef BRANCHMANAGER_H
#define BRANCHMANAGER_H

#include "employee.h"
#include "accesspolicy.h"
#include <cstddef>

class BranchManager : public Employee, protected AccessPolicy {
private:
    std::size_t assignedCount_;
    std::size_t startedShifts_;
    std::size_t endedShifts_;

    void ensureCanManage(const PostalBranch &branch, const Employee &employee,
                         const std::string &permission);

public:
    BranchManager(int id, const std::string &name, int branchId);

    using AccessPolicy::enabled;
    using AccessPolicy::deniedCount;

    std::string role() const override;
    std::size_t assignedCount() const;
    std::size_t startedShifts() const;
    std::size_t endedShifts() const;
    void assignEmployee(const PostalBranch &branch, Employee &employee);
    void startEmployeeShift(const PostalBranch &branch, Employee &employee);
    void endEmployeeShift(const PostalBranch &branch, Employee &employee);
    void suspendManagement();
    void restoreManagement();
};

#endif
