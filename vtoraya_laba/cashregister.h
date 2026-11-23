#ifndef CASHREGISTER_H
#define CASHREGISTER_H

#include "receipt.h"
#include "refund.h"
#include "shipment.h"
#include <string>
#include <vector>

class CashRegister {
private:
    int id_;
    int branchId_;
    int balance_;
    bool open_;
    std::vector<Payment> payments_;
    std::vector<Refund> processedRefunds_;

    void ensureOpen() const;
    void ensureUnpaid(int paymentId, const std::string &trackingNumber) const;
    Payment &findMutablePayment(int paymentId);

public:
    CashRegister(int id, int branchId);

    int id() const;
    int branchId() const;
    int balance() const;
    bool isOpen() const;
    void openShift();
    void closeShift();
    Receipt acceptPayment(int paymentId, const Shipment &shipment);
    const Payment &findPayment(int paymentId) const;
    const Refund &findRefund(int refundId) const;
    void processRefund(const Refund &refund);
};

#endif
