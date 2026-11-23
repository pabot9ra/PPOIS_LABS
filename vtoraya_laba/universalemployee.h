#ifndef UNIVERSALEMPLOYEE_H
#define UNIVERSALEMPLOYEE_H

#include "postalclerk.h"
#include "courier.h"

class UniversalEmployee : public PostalClerk, public Courier {
public:
    UniversalEmployee(int id, const std::string &name, int branchId);

    std::string role() const override;
};

#endif
