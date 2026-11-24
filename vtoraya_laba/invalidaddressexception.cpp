#include "invalidaddressexception.h"

InvalidAddressException::InvalidAddressException(const std::string &field, const std::string &message)
    : PostalException("Invalid address (" + field + "): " + message), field_(field) {
}

const std::string &InvalidAddressException::field() const {
    return field_;
}
