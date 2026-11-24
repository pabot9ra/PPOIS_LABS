#include "capacityexceededexception.h"

CapacityExceededException::CapacityExceededException(const std::string &object, int requiredWeight, int availableWeight)
    : PostalException(object + ": required " + std::to_string(requiredWeight) +
                      " g, available " + std::to_string(availableWeight) + " g"),
      requiredWeight_(requiredWeight), availableWeight_(availableWeight) {
}

int CapacityExceededException::requiredWeight() const {
    return requiredWeight_;
}

int CapacityExceededException::availableWeight() const {
    return availableWeight_;
}
