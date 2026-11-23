#ifndef CASHIER_H
#define CASHIER_H

#include "employee.h"
#include "cashregister.h"

class Cashier : public Employee, private CashRegister {
public:
    Cashier(int id, const std::string &name, int branchId, int registerId);

    using Employee::id;
    using Employee::branchId;
    using CashRegister::balance;
    using CashRegister::findPayment;
    using CashRegister::findRefund;

    std::string role() const override;
    void startShift() override;
    void endShift() override;
    void assignToBranch(int branchId) override;
    Receipt acceptPayment(const PostalBranch &branch, int paymentId, const Shipment &shipment);
    void processRefund(const PostalBranch &branch, const Refund &refund);
};

#endif
