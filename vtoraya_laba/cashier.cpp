#include "cashier.h"
#include <stdexcept>

Cashier::Cashier(int id, const std::string &name, int branchId, int registerId)
    : Employee(id, name, branchId), CashRegister(registerId, branchId) {
}

std::string Cashier::role() const {
    return "Cashier";
}

void Cashier::startShift() {
    if(CashRegister::isOpen()){
        throw std::logic_error("Cashier register shift is already open");
    }
    Employee::startShift();
    CashRegister::openShift();
}

void Cashier::endShift() {
    if(!Employee::onShift()){
        throw std::logic_error("Cashier is not on shift");
    }
    CashRegister::closeShift();
    Employee::endShift();
}

void Cashier::assignToBranch(int branchId) {
    if(branchId != CashRegister::branchId()){
        throw std::invalid_argument("Cashier is bound to the branch of this register");
    }
    Employee::assignToBranch(branchId);
}

Receipt Cashier::acceptPayment(const PostalBranch &branch, int paymentId, const Shipment &shipment) {
    ensureCanWork(branch);
    return CashRegister::acceptPayment(paymentId, shipment);
}

void Cashier::processRefund(const PostalBranch &branch, const Refund &refund) {
    ensureCanWork(branch);
    CashRegister::processRefund(refund);
}
