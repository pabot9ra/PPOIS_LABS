#ifndef DUPLICATESHIPMENTEXCEPTION_H
#define DUPLICATESHIPMENTEXCEPTION_H

#include "postalexception.h"

class DuplicateShipmentException : public PostalException {
private:
    std::string trackingNumber_;

public:
    explicit DuplicateShipmentException(const std::string &trackingNumber);

    const std::string &trackingNumber() const;
};

#endif
