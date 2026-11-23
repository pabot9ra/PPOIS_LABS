#include "universalemployee.h"

UniversalEmployee::UniversalEmployee(int id, const std::string &name, int branchId)
    : Employee(id, name, branchId), PostalClerk(id, name, branchId), Courier(id, name, branchId) {
}

std::string UniversalEmployee::role() const {
    return "Postal clerk and courier";
}
