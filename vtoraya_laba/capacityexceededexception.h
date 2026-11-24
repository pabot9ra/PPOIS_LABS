#ifndef CAPACITYEXCEEDEDEXCEPTION_H
#define CAPACITYEXCEEDEDEXCEPTION_H

#include "postalexception.h"

class CapacityExceededException : public PostalException {
private:
    int requiredWeight_;
    int availableWeight_;

public:
    CapacityExceededException(const std::string &object, int requiredWeight, int availableWeight);

    int requiredWeight() const;
    int availableWeight() const;
};

#endif
