#ifndef BRANCHNOTFOUNDEXCEPTION_H
#define BRANCHNOTFOUNDEXCEPTION_H

#include "postalexception.h"

class BranchNotFoundException : public PostalException {
private:
    int branchId_;

public:
    explicit BranchNotFoundException(int branchId);

    int branchId() const;
};

#endif
