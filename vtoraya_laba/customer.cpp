#include "customer.h"
#include "invalidcustomerexception.h"
#include <cctype>
#include <stdexcept>

Customer::Customer(int id, const std::string &name, const PostalAddress &address)
    : id_(id), name_(name), address_(address) {
    if(id_ <= 0){
        throw InvalidCustomerException(id_, "Customer ID must be positive");
    }
    bool hasName = false;
    for(char character : name_){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasName = true;
            break;
        }
    }
    if(!hasName){
        throw InvalidCustomerException(id_, "Customer name must not be blank");
    }
}

int Customer::id() const {
    return id_;
}

const std::string &Customer::name() const {
    return name_;
}

const PostalAddress &Customer::address() const {
    return address_;
}

void Customer::changeAddress(const PostalAddress &address) {
    address_ = address;
}
