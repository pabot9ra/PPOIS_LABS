#include "cashregister.h"
#include "paymentnotfoundexception.h"
#include "refundlimitexception.h"
#include <limits>
#include <stdexcept>

CashRegister::CashRegister(int id, int branchId)
    : id_(id), branchId_(branchId), balance_(0), open_(false) {
    if(id_ <= 0 || branchId_ <= 0){
        throw std::invalid_argument("Register and branch IDs must be positive");
    }
}

int CashRegister::id() const {
    return id_;
}

int CashRegister::branchId() const {
    return branchId_;
}

int CashRegister::balance() const {
    return balance_;
}

bool CashRegister::isOpen() const {
    return open_;
}

void CashRegister::openShift() {
    if(open_){
        throw std::logic_error("Register shift is already open");
    }
    open_ = true;
}

void CashRegister::closeShift() {
    ensureOpen();
    open_ = false;
}

void CashRegister::ensureOpen() const {
    if(!open_){
        throw std::logic_error("Register shift must be open");
    }
}

void CashRegister::ensureUnpaid(int paymentId, const std::string &trackingNumber) const {
    for(const Payment &payment : payments_){
        if(payment.id() == paymentId ||
           (payment.trackingNumber() == trackingNumber && payment.refundableAmount() > 0)){
            throw std::invalid_argument("Payment ID is already used or shipment is already paid");
        }
    }
}

Receipt CashRegister::acceptPayment(int paymentId, const Shipment &shipment) {
    ensureOpen();
    ensureUnpaid(paymentId, shipment.trackingNumber());
    int amount = shipment.calculatePrice();
    if(amount > std::numeric_limits<int>::max() - balance_){
        throw std::overflow_error("Register balance is too large");
    }
    Payment payment(paymentId, shipment.trackingNumber(), amount);
    payment.capture();
    Receipt receipt(id_, payment);
    payments_.push_back(payment);
    balance_ += amount;
    return receipt;
}

const Payment &CashRegister::findPayment(int paymentId) const {
    for(const Payment &payment : payments_){
        if(payment.id() == paymentId){
            return payment;
        }
    }
    throw PaymentNotFoundException(paymentId);
}

Payment &CashRegister::findMutablePayment(int paymentId) {
    for(Payment &payment : payments_){
        if(payment.id() == paymentId){
            return payment;
        }
    }
    throw PaymentNotFoundException(paymentId);
}

void CashRegister::processRefund(const Refund &refund) {
    ensureOpen();
    for(const Refund &processed : processedRefunds_){
        if(processed.id() == refund.id()){
            throw std::invalid_argument("Refund is already processed");
        }
    }
    Payment &payment = findMutablePayment(refund.paymentId());
    int available = payment.refundableAmount();
    if(balance_ < available){
        available = balance_;
    }
    if(refund.amount() > available){
        throw RefundLimitException(refund.amount(), available);
    }
    processedRefunds_.push_back(refund);
    payment.refund(refund.amount());
    balance_ -= refund.amount();
}

const Refund &CashRegister::findRefund(int refundId) const {
    for(const Refund &refund : processedRefunds_){
        if(refund.id() == refundId){
            return refund;
        }
    }
    throw std::out_of_range("Refund was not found");
}
