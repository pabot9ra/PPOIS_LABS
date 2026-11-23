#include "postalexception.h"

PostalException::PostalException(const std::string &message)
    : std::runtime_error(message) {
}
