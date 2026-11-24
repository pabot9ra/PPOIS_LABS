#include <UnitTest++/UnitTest++.h>
#include "invalidaddressexception.h"
#include "invalidcustomerexception.h"
#include "duplicateshipmentexception.h"
#include "shipmentstateexception.h"
#include "capacityexceededexception.h"
#include "paymentnotfoundexception.h"
#include "refundlimitexception.h"
#include "branchnotfoundexception.h"
#include "customernotfoundexception.h"
#include "accessdeniedexception.h"
#include "postalnetwork.h"
#include "mailbag.h"
#include "vehicle.h"
#include "letter.h"
#include "cashregister.h"
#include "accesspolicy.h"
#include <string>
#include <type_traits>

static_assert(std::is_base_of<PostalException,InvalidAddressException>::value, "Address error needs common base");
static_assert(std::is_base_of<PostalException,InvalidCustomerException>::value, "Customer error needs common base");
static_assert(std::is_base_of<PostalException,DuplicateShipmentException>::value, "Duplicate error needs common base");
static_assert(std::is_base_of<PostalException,ShipmentStateException>::value, "State error needs common base");
static_assert(std::is_base_of<PostalException,CapacityExceededException>::value, "Capacity error needs common base");
static_assert(std::is_base_of<PostalException,PaymentNotFoundException>::value, "Payment error needs common base");
static_assert(std::is_base_of<PostalException,RefundLimitException>::value, "Refund error needs common base");
static_assert(std::is_base_of<PostalException,BranchNotFoundException>::value, "Branch error needs common base");
static_assert(std::is_base_of<PostalException,CustomerNotFoundException>::value, "Customer lookup error needs common base");
static_assert(std::is_base_of<PostalException,AccessDeniedException>::value, "Access error needs common base");

struct DomainErrorFixture {
    PostalAddress address{"Минск","Гикало","9","220013"};
    PostalBranch branch{13,address};
    Customer sender{1,"Владислав",address};
    Customer recipient{2,"Алексей",address};
    Letter letter{"LT001",sender,recipient,20,5};
};

TEST(InvalidAddressExceptionReportsTheRejectedField) {
    bool caught=false;
    try { PostalAddress address("Минск","Гикало","9","ABC"); }
    catch(const InvalidAddressException &error){
        caught=true;
        CHECK_EQUAL(std::string("postalCode"),error.field());
        CHECK(std::string(error.what()).find("postalCode")!=std::string::npos);
    }
    CHECK(caught);
    CHECK_THROW(PostalAddress("","Гикало","9","220013"),PostalException);
}

TEST_FIXTURE(DomainErrorFixture, InvalidCustomerExceptionPreservesProblemId) {
    bool caught=false;
    try { Customer invalid(-7,"Владислав",address); }
    catch(const InvalidCustomerException &error){
        caught=true;
        CHECK_EQUAL(-7,error.customerId());
        CHECK(std::string(error.what()).find("-7")!=std::string::npos);
    }
    CHECK(caught);
    CHECK_THROW(Customer(7,"",address),PostalException);
}

TEST_FIXTURE(DomainErrorFixture, DuplicateShipmentExceptionPreservesTrackingNumber) {
    branch.acceptShipment(letter);
    bool caught=false;
    try { branch.acceptShipment(letter); }
    catch(const DuplicateShipmentException &error){
        caught=true;
        CHECK_EQUAL(std::string("LT001"),error.trackingNumber());
    }
    CHECK(caught);
    CHECK_EQUAL(std::size_t(1),branch.shipmentCount());
}

TEST_FIXTURE(DomainErrorFixture, ShipmentStateExceptionReportsOperationAndActualState) {
    bool caught=false;
    try { letter.deliver(); }
    catch(const ShipmentStateException &error){
        caught=true;
        CHECK_EQUAL(std::string("LT001"),error.trackingNumber());
        CHECK_EQUAL(std::string("deliver"),error.operation());
        CHECK(error.currentStatus()==ShipmentStatus::Accepted);
    }
    CHECK(caught);
    CHECK(letter.status()==ShipmentStatus::Accepted);
    letter.dispatch();
    CHECK_THROW(letter.dispatch(),PostalException);
}

TEST_FIXTURE(DomainErrorFixture, CapacityExceededExceptionReportsRequiredAndAvailableWeight) {
    MailBag bag("B1",13,14,10);
    bool caught=false;
    try { bag.addShipment(letter); }
    catch(const CapacityExceededException &error){
        caught=true;
        CHECK_EQUAL(20,error.requiredWeight());
        CHECK_EQUAL(10,error.availableWeight());
        CHECK(std::string(error.what()).find("B1")!=std::string::npos);
    }
    CHECK(caught);
    CHECK_EQUAL(0,bag.weightGrams());
}

TEST_FIXTURE(DomainErrorFixture, VehicleCapacityErrorReportsRemainingCapacity) {
    Route route(1,"route");
    PostalBranch destination(14,address);
    route.addStop(RouteStop(branch,0,0));
    route.addStop(RouteStop(destination,1,1));
    Vehicle vehicle("1234 AB-7",10,route);
    MailBag bag("B1",13,14,100);
    bag.addShipment(letter);
    bag.seal();
    CHECK_THROW(vehicle.loadBag(bag),CapacityExceededException);
    CHECK_EQUAL(std::size_t(0),vehicle.bagCount());
}

TEST_FIXTURE(DomainErrorFixture, PaymentNotFoundExceptionPreservesRequestedPaymentId) {
    CashRegister cash(1,13);
    bool caught=false;
    try { cash.findPayment(77); }
    catch(const PaymentNotFoundException &error){
        caught=true;
        CHECK_EQUAL(77,error.paymentId());
    }
    CHECK(caught);
    cash.openShift();
    Refund refund(1,77,10,"reason");
    CHECK_THROW(cash.processRefund(refund),PaymentNotFoundException);
}

TEST(RefundLimitExceptionPreservesRequestedAndRemainingAmounts) {
    Payment payment(1,"LT001",100);
    payment.capture();
    payment.refund(40);
    bool caught=false;
    try { payment.refund(61); }
    catch(const RefundLimitException &error){
        caught=true;
        CHECK_EQUAL(61,error.requestedAmount());
        CHECK_EQUAL(60,error.availableAmount());
    }
    CHECK(caught);
    CHECK_EQUAL(60,payment.refundableAmount());
}

TEST_FIXTURE(DomainErrorFixture, CashRegisterRefundLimitPreservesBalanceAndHistory) {
    CashRegister cash(1,13);
    cash.openShift();
    cash.acceptPayment(1,letter);
    Refund excess(1,1,101,"reason");
    CHECK_THROW(cash.processRefund(excess),RefundLimitException);
    CHECK_EQUAL(100,cash.balance());
    CHECK_THROW(cash.findRefund(1),std::out_of_range);
}

TEST(BranchNotFoundExceptionPreservesRequestedBranchId) {
    BranchDirectory branches;
    bool caught=false;
    try { branches.find(99); }
    catch(const BranchNotFoundException &error){ caught=true; CHECK_EQUAL(99,error.branchId()); }
    CHECK(caught);
    CHECK_THROW(branches.remove(99),BranchNotFoundException);
}

TEST_FIXTURE(DomainErrorFixture, CustomerNotFoundExceptionPreservesRequestedCustomerId) {
    CustomerDirectory customers;
    bool caught=false;
    try { customers.find(99); }
    catch(const CustomerNotFoundException &error){ caught=true; CHECK_EQUAL(99,error.customerId()); }
    CHECK(caught);
    CHECK_THROW(customers.changeAddress(99,address),CustomerNotFoundException);
    CHECK_THROW(customers.remove(99),CustomerNotFoundException);
}

TEST(AccessDeniedExceptionPreservesPermissionAndPolicyDenialCount) {
    AccessPolicy policy;
    bool caught=false;
    try { policy.checkPermission("assign_employee"); }
    catch(const AccessDeniedException &error){
        caught=true;
        CHECK_EQUAL(std::string("assign_employee"),error.permission());
    }
    CHECK(caught);
    CHECK_EQUAL(std::size_t(1),policy.deniedCount());
    CHECK_THROW(policy.checkPermission("assign_employee"),PostalException);
}
