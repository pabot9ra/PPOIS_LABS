#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "postaladdress.h"
#include <string>

class Customer {
private:
    int id_;
    std::string name_;
    PostalAddress address_;

public:
    Customer(int id, const std::string &name, const PostalAddress &address);

    int id() const;
    const std::string &name() const;
    const PostalAddress &address() const;

    void changeAddress(const PostalAddress &address);
};

#endif
