#include "accessdeniedexception.h"

AccessDeniedException::AccessDeniedException(const std::string &permission)
    : PostalException("Access denied for permission: " + permission), permission_(permission) {
}

const std::string &AccessDeniedException::permission() const {
    return permission_;
}
