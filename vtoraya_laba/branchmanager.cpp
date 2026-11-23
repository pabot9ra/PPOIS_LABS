#include "branchmanager.h"
#include <stdexcept>

namespace {
const std::string ASSIGN_EMPLOYEE = "assign_employee";
const std::string MANAGE_SHIFT = "manage_shift";
}

BranchManager::BranchManager(int id, const std::string &name, int branchId)
    : Employee(id, name, branchId), assignedCount_(0), startedShifts_(0), endedShifts_(0) {
    grantPermission(ASSIGN_EMPLOYEE);
    grantPermission(MANAGE_SHIFT);
}

std::string BranchManager::role() const {
    return "Branch manager";
}

std::size_t BranchManager::assignedCount() const {
    return assignedCount_;
}

std::size_t BranchManager::startedShifts() const {
    return startedShifts_;
}

std::size_t BranchManager::endedShifts() const {
    return endedShifts_;
}

void BranchManager::ensureCanManage(const PostalBranch &branch, const Employee &employee,
                                    const std::string &permission) {
    ensureCanWork(branch);
    checkPermission(permission);
    if(&employee == this){
        throw std::logic_error("Manager must use personal shift methods for themselves");
    }
}

void BranchManager::assignEmployee(const PostalBranch &branch, Employee &employee) {
    ensureCanManage(branch, employee, ASSIGN_EMPLOYEE);
    employee.assignToBranch(branch.id());
    assignedCount_++;
}

void BranchManager::startEmployeeShift(const PostalBranch &branch, Employee &employee) {
    ensureCanManage(branch, employee, MANAGE_SHIFT);
    if(employee.branchId() != branch.id()){
        throw std::invalid_argument("Employee belongs to a different branch");
    }
    employee.startShift();
    startedShifts_++;
}

void BranchManager::endEmployeeShift(const PostalBranch &branch, Employee &employee) {
    ensureCanManage(branch, employee, MANAGE_SHIFT);
    if(employee.branchId() != branch.id()){
        throw std::invalid_argument("Employee belongs to a different branch");
    }
    employee.endShift();
    endedShifts_++;
}

void BranchManager::suspendManagement() {
    disable();
}

void BranchManager::restoreManagement() {
    enable();
}
