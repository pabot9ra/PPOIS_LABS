#include "customerdirectory.h"
#include "customernotfoundexception.h"
#include <stdexcept>

std::size_t CustomerDirectory::size() const {
    return customers_.size();
}

bool CustomerDirectory::contains(int id) const {
    return customers_.count(id) > 0;
}

void CustomerDirectory::add(const Customer &customer) {
    if(contains(customer.id())){
        throw std::invalid_argument("Customer ID is already registered");
    }
    customers_.emplace(customer.id(), customer);
}

const Customer &CustomerDirectory::find(int id) const {
    auto it = customers_.find(id);
    if(it == customers_.end()){
        throw CustomerNotFoundException(id);
    }
    return it->second;
}

void CustomerDirectory::changeAddress(int id, const PostalAddress &address) {
    auto it = customers_.find(id);
    if(it == customers_.end()){
        throw CustomerNotFoundException(id);
    }
    it->second.changeAddress(address);
}

void CustomerDirectory::remove(int id) {
    if(customers_.erase(id) == 0){
        throw CustomerNotFoundException(id);
    }
}
