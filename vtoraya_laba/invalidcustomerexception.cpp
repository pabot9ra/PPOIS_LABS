#include "invalidcustomerexception.h"

InvalidCustomerException::InvalidCustomerException(int customerId, const std::string &message)
    : PostalException("Invalid customer " + std::to_string(customerId) + ": " + message), customerId_(customerId) {
}

int InvalidCustomerException::customerId() const {
    return customerId_;
}
