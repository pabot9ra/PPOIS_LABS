#include "refundlimitexception.h"

RefundLimitException::RefundLimitException(int requestedAmount, int availableAmount)
    : PostalException("Refund requested: " + std::to_string(requestedAmount) +
                      " kopecks, available: " + std::to_string(availableAmount) + " kopecks"),
      requestedAmount_(requestedAmount), availableAmount_(availableAmount) {
}

int RefundLimitException::requestedAmount() const {
    return requestedAmount_;
}

int RefundLimitException::availableAmount() const {
    return availableAmount_;
}
