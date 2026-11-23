#include "employee.h"
#include <cctype>
#include <stdexcept>

Employee::Employee(int id, const std::string &name, int branchId)
    : id_(id), name_(name), branchId_(branchId), onShift_(false) {
    if(id_ <= 0 || branchId_ <= 0){
        throw std::invalid_argument("Employee and branch IDs must be positive");
    }
    bool hasName = false;
    for(char character : name_){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasName = true;
            break;
        }
    }
    if(!hasName){
        throw std::invalid_argument("Employee name must not be blank");
    }
}

int Employee::id() const {
    return id_;
}

const std::string &Employee::name() const {
    return name_;
}

int Employee::branchId() const {
    return branchId_;
}

bool Employee::onShift() const {
    return onShift_;
}

void Employee::startShift() {
    if(onShift_){
        throw std::logic_error("Employee is already on shift");
    }
    onShift_ = true;
}

void Employee::endShift() {
    if(!onShift_){
        throw std::logic_error("Employee is not on shift");
    }
    onShift_ = false;
}

void Employee::assignToBranch(int branchId) {
    if(branchId <= 0){
        throw std::invalid_argument("Branch ID must be positive");
    }
    if(onShift_){
        throw std::logic_error("Cannot transfer an employee during a shift");
    }
    branchId_ = branchId;
}

void Employee::ensureCanWork(const PostalBranch &branch) const {
    if(!onShift_){
        throw std::logic_error("Employee must start a shift before working");
    }
    if(branch.id() != branchId_){
        throw std::logic_error("Employee is assigned to a different branch");
    }
}
