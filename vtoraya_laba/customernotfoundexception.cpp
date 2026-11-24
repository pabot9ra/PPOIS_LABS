#include "customernotfoundexception.h"

CustomerNotFoundException::CustomerNotFoundException(int customerId)
    : PostalException("Customer was not found: " + std::to_string(customerId)), customerId_(customerId) {
}

int CustomerNotFoundException::customerId() const {
    return customerId_;
}
