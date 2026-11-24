#include "paymentnotfoundexception.h"
#include "refundlimitexception.h"
#include <UnitTest++/UnitTest++.h>
#include "cashregister.h"
#include "letter.h"
#include "parcel.h"
#include <limits>
#include <stdexcept>
#include <string>

struct PaymentFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    Parcel parcel{"PK001", sender, recipient, 1500, 200, 100};
    CashRegister cash{1, 13};
};

TEST(PaymentStoresIdentityAndRequiresCapture) {
    Payment payment(1, "LT001", 100);
    CHECK_EQUAL(1, payment.id());
    CHECK_EQUAL(std::string("LT001"), payment.trackingNumber());
    CHECK_EQUAL(100, payment.amount());
    CHECK(!payment.captured());
    CHECK_EQUAL(0, payment.refundableAmount());
    CHECK_THROW(payment.refund(1), std::logic_error);
    payment.capture();
    CHECK(payment.captured());
    CHECK_EQUAL(100, payment.refundableAmount());
    CHECK_THROW(payment.capture(), std::logic_error);
}

TEST(PaymentRejectsInvalidInput) {
    CHECK_THROW(Payment(0, "LT001", 100), std::invalid_argument);
    CHECK_THROW(Payment(-1, "LT001", 100), std::invalid_argument);
    CHECK_THROW(Payment(1, "LT001", 0), std::invalid_argument);
    CHECK_THROW(Payment(1, "LT001", -1), std::invalid_argument);
    CHECK_THROW(Payment(1, "", 100), std::invalid_argument);
    CHECK_THROW(Payment(1, "LT 001", 100), std::invalid_argument);
    CHECK_THROW(Payment(1, "LT001\n", 100), std::invalid_argument);
}

TEST(PaymentAllowsPartialRefundsWithinOriginalAmount) {
    Payment payment(1, "LT001", 100);
    payment.capture();
    CHECK_THROW(payment.refund(0), RefundLimitException);
    CHECK_THROW(payment.refund(-1), RefundLimitException);
    CHECK_THROW(payment.refund(101), RefundLimitException);
    payment.refund(40);
    CHECK_EQUAL(60, payment.refundableAmount());
    CHECK_THROW(payment.refund(61), RefundLimitException);
    payment.refund(60);
    CHECK_EQUAL(0, payment.refundableAmount());
    CHECK_THROW(payment.refund(1), RefundLimitException);
}

TEST(ReceiptRequiresCapturedPaymentAndFormatsSnapshot) {
    Payment payment(7, "LT001", 100);
    CHECK_THROW(Receipt(1, payment), std::logic_error);
    payment.capture();
    CHECK_THROW(Receipt(0, payment), std::invalid_argument);
    Receipt receipt(3, payment);
    CHECK_EQUAL(3, receipt.registerId());
    CHECK_EQUAL(7, receipt.paymentId());
    CHECK_EQUAL(std::string("LT001"), receipt.trackingNumber());
    CHECK_EQUAL(100, receipt.amount());
    CHECK_EQUAL(std::string("Register 3, payment 7, shipment LT001, amount 100 kopecks"), receipt.format());
    payment.refund(50);
    CHECK_EQUAL(100, receipt.amount());
}

TEST(RefundStoresRequestAndRejectsInvalidFields) {
    Refund refund(5, 7, 50, "Отмена услуги");
    CHECK_EQUAL(5, refund.id());
    CHECK_EQUAL(7, refund.paymentId());
    CHECK_EQUAL(50, refund.amount());
    CHECK_EQUAL(std::string("Отмена услуги"), refund.reason());
    CHECK_THROW(Refund(0, 7, 50, "reason"), std::invalid_argument);
    CHECK_THROW(Refund(5, 0, 50, "reason"), std::invalid_argument);
    CHECK_THROW(Refund(5, 7, 0, "reason"), std::invalid_argument);
    CHECK_THROW(Refund(5, 7, -1, "reason"), std::invalid_argument);
    CHECK_THROW(Refund(5, 7, 50, ""), std::invalid_argument);
    CHECK_THROW(Refund(5, 7, 50, " \t\n"), std::invalid_argument);
}

TEST_FIXTURE(PaymentFixture, CashRegisterValidatesIdentityAndShift) {
    CHECK_THROW(CashRegister(0, 13), std::invalid_argument);
    CHECK_THROW(CashRegister(1, 0), std::invalid_argument);
    CHECK_EQUAL(1, cash.id());
    CHECK_EQUAL(13, cash.branchId());
    CHECK_EQUAL(0, cash.balance());
    CHECK(!cash.isOpen());
    CHECK_THROW(cash.closeShift(), std::logic_error);
    CHECK_THROW(cash.acceptPayment(1, letter), std::logic_error);
    Refund refund(1, 1, 10, "reason");
    CHECK_THROW(cash.processRefund(refund), std::logic_error);
    cash.openShift();
    CHECK(cash.isOpen());
    CHECK_THROW(cash.openShift(), std::logic_error);
    cash.closeShift();
    CHECK(!cash.isOpen());
}

TEST_FIXTURE(PaymentFixture, CashRegisterUsesPolymorphicShipmentPrices) {
    cash.openShift();
    Receipt first = cash.acceptPayment(1, letter);
    Receipt second = cash.acceptPayment(2, parcel);
    CHECK_EQUAL(100, first.amount());
    CHECK_EQUAL(400, second.amount());
    CHECK_EQUAL(500, cash.balance());
    const Payment &payment = cash.findPayment(2);
    CHECK(payment.captured());
    CHECK_EQUAL(std::string("PK001"), payment.trackingNumber());
}

TEST_FIXTURE(PaymentFixture, CashRegisterRejectsDuplicateIdAndDuplicatePayment) {
    cash.openShift();
    cash.acceptPayment(1, letter);
    CHECK_THROW(cash.acceptPayment(1, parcel), std::invalid_argument);
    CHECK_THROW(cash.acceptPayment(2, letter), std::invalid_argument);
    CHECK_EQUAL(100, cash.balance());
    CHECK_THROW(cash.findPayment(2), PaymentNotFoundException);
}

TEST_FIXTURE(PaymentFixture, CashRegisterRejectsInvalidPaymentIdWithoutChangingBalance) {
    cash.openShift();
    CHECK_THROW(cash.acceptPayment(0, letter), std::invalid_argument);
    CHECK_EQUAL(0, cash.balance());
    CHECK_THROW(cash.findPayment(0), PaymentNotFoundException);
}

TEST_FIXTURE(PaymentFixture, CashRegisterSupportsPartialRefundAndRejectsDuplicateRequest) {
    cash.openShift();
    cash.acceptPayment(1, letter);
    Refund refund(1, 1, 40, "Частичный возврат");
    cash.processRefund(refund);
    CHECK_EQUAL(60, cash.balance());
    CHECK_EQUAL(60, cash.findPayment(1).refundableAmount());
    CHECK_THROW(cash.processRefund(refund), std::invalid_argument);
    CHECK_EQUAL(60, cash.balance());
    Refund remainder(2, 1, 60, "Остаток");
    cash.processRefund(remainder);
    CHECK_EQUAL(0, cash.balance());
}

TEST_FIXTURE(PaymentFixture, CashRegisterRejectsMissingOrExcessRefundWithoutMutation) {
    cash.openShift();
    cash.acceptPayment(1, letter);
    Refund missing(1, 99, 10, "reason");
    Refund excess(1, 1, 101, "reason");
    CHECK_THROW(cash.processRefund(missing), PaymentNotFoundException);
    CHECK_THROW(cash.processRefund(excess), RefundLimitException);
    CHECK_EQUAL(100, cash.balance());
    CHECK_EQUAL(100, cash.findPayment(1).refundableAmount());
    Refund valid(1, 1, 100, "reason");
    cash.processRefund(valid);
    CHECK_EQUAL(0, cash.balance());
}

TEST_FIXTURE(PaymentFixture, CashRegisterAllowsRepaymentAfterFullRefund) {
    cash.openShift();
    cash.acceptPayment(1, letter);
    Refund refund(1, 1, 100, "Полный возврат");
    cash.processRefund(refund);
    Receipt receipt = cash.acceptPayment(2, letter);
    CHECK_EQUAL(2, receipt.paymentId());
    CHECK_EQUAL(100, cash.balance());
    CHECK_THROW(cash.acceptPayment(1, letter), std::invalid_argument);
}

TEST_FIXTURE(PaymentFixture, CashRegisterRejectsRepaymentAfterPartialRefund) {
    cash.openShift();
    cash.acceptPayment(1, letter);
    Refund refund(1, 1, 40, "reason");
    cash.processRefund(refund);
    CHECK_THROW(cash.acceptPayment(2, letter), std::invalid_argument);
    CHECK_EQUAL(60, cash.balance());
}

TEST_FIXTURE(PaymentFixture, CashRegisterPreservesRecordsBetweenShifts) {
    cash.openShift();
    cash.acceptPayment(1, letter);
    cash.closeShift();
    CHECK_EQUAL(100, cash.balance());
    CHECK_EQUAL(100, cash.findPayment(1).amount());
    CHECK_THROW(cash.acceptPayment(2, parcel), std::logic_error);
    cash.openShift();
    cash.acceptPayment(2, parcel);
    CHECK_EQUAL(500, cash.balance());
}

TEST_FIXTURE(PaymentFixture, CashRegisterChecksBalanceOverflowBeforeRecordingPayment) {
    cash.openShift();
    int maximum = std::numeric_limits<int>::max();
    Letter expensive("MAX001", sender, recipient, 1, maximum);
    cash.acceptPayment(1, expensive);
    CHECK_EQUAL(maximum, cash.balance());
    CHECK_THROW(cash.acceptPayment(2, letter), std::overflow_error);
    CHECK_EQUAL(maximum, cash.balance());
    CHECK_THROW(cash.findPayment(2), PaymentNotFoundException);
}

TEST_FIXTURE(PaymentFixture, CashRegisterPreservesRefundReasonAndOriginalAmount) {
    CHECK_THROW(cash.findRefund(1), std::out_of_range);
    cash.openShift();
    cash.acceptPayment(1, letter);
    Refund refund(1, 1, 40, "Отмена дополнительной услуги");
    cash.processRefund(refund);
    const Refund &stored = cash.findRefund(1);
    CHECK_EQUAL(1, stored.paymentId());
    CHECK_EQUAL(40, stored.amount());
    CHECK_EQUAL(std::string("Отмена дополнительной услуги"), stored.reason());
    CHECK_EQUAL(100, cash.findPayment(1).amount());
    cash.closeShift();
    cash.openShift();
    CHECK_THROW(cash.processRefund(refund), std::invalid_argument);
    CHECK_EQUAL(60, cash.balance());
}
