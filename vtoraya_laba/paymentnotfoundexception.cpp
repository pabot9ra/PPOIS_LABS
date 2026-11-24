#include "paymentnotfoundexception.h"

PaymentNotFoundException::PaymentNotFoundException(int paymentId)
    : PostalException("Payment was not found: " + std::to_string(paymentId)), paymentId_(paymentId) {
}

int PaymentNotFoundException::paymentId() const {
    return paymentId_;
}
