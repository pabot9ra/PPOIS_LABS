#ifndef CUSTOMERDIRECTORY_H
#define CUSTOMERDIRECTORY_H

#include "customer.h"
#include <cstddef>
#include <map>

class CustomerDirectory {
private:
    std::map<int, Customer> customers_;

public:
    std::size_t size() const;
    bool contains(int id) const;
    void add(const Customer &customer);
    const Customer &find(int id) const;
    void changeAddress(int id, const PostalAddress &address);
    void remove(int id);
};

#endif
