#include "branchnotfoundexception.h"

BranchNotFoundException::BranchNotFoundException(int branchId)
    : PostalException("Postal branch was not found: " + std::to_string(branchId)), branchId_(branchId) {
}

int BranchNotFoundException::branchId() const {
    return branchId_;
}
