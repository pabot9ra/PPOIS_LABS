#ifndef ACCESSDENIEDEXCEPTION_H
#define ACCESSDENIEDEXCEPTION_H

#include "postalexception.h"

class AccessDeniedException : public PostalException {
private:
    std::string permission_;

public:
    explicit AccessDeniedException(const std::string &permission);

    const std::string &permission() const;
};

#endif
