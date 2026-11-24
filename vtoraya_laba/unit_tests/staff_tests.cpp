#include "duplicateshipmentexception.h"
#include "shipmentstateexception.h"
#include <UnitTest++/UnitTest++.h>
#include "postalclerk.h"
#include "courier.h"
#include "letter.h"
#include "shipmentnotfoundexception.h"
#include <stdexcept>
#include <string>
#include <type_traits>

static_assert(std::is_abstract<Employee>::value, "Employee must be abstract");
static_assert(std::has_virtual_destructor<Employee>::value, "Employee needs virtual destruction");

struct StaffFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    PostalBranch branch{13, address};
    PostalBranch otherBranch{14, address};
    PostalClerk clerk{10, "Ольга", 13};
    Courier courier{11, "Иван", 13};
};

TEST_FIXTURE(StaffFixture, EmployeeStoresIdentityAndStartsOffShift) {
    CHECK_EQUAL(10, clerk.id());
    CHECK_EQUAL(std::string("Ольга"), clerk.name());
    CHECK_EQUAL(13, clerk.branchId());
    CHECK(!clerk.onShift());
}

TEST(EmployeeRejectsInvalidIdentity) {
    CHECK_THROW(PostalClerk(0, "Ольга", 13), std::invalid_argument);
    CHECK_THROW(PostalClerk(-1, "Ольга", 13), std::invalid_argument);
    CHECK_THROW(PostalClerk(10, "Ольга", 0), std::invalid_argument);
    CHECK_THROW(PostalClerk(10, "Ольга", -1), std::invalid_argument);
    CHECK_THROW(Courier(11, "", 13), std::invalid_argument);
    CHECK_THROW(Courier(11, " \t\n", 13), std::invalid_argument);
}

TEST_FIXTURE(StaffFixture, EmployeeShiftTransitionsAreValidated) {
    CHECK_THROW(clerk.endShift(), std::logic_error);
    clerk.startShift();
    CHECK(clerk.onShift());
    CHECK_THROW(clerk.startShift(), std::logic_error);
    CHECK(clerk.onShift());
    clerk.endShift();
    CHECK(!clerk.onShift());
    CHECK_THROW(clerk.endShift(), std::logic_error);
}

TEST_FIXTURE(StaffFixture, EmployeeCanTransferOnlyBetweenShifts) {
    CHECK_THROW(clerk.assignToBranch(0), std::invalid_argument);
    CHECK_THROW(clerk.assignToBranch(-1), std::invalid_argument);
    CHECK_EQUAL(13, clerk.branchId());
    clerk.startShift();
    CHECK_THROW(clerk.assignToBranch(14), std::logic_error);
    CHECK_EQUAL(13, clerk.branchId());
    clerk.endShift();
    clerk.assignToBranch(14);
    CHECK_EQUAL(14, clerk.branchId());
}

TEST_FIXTURE(StaffFixture, ClerkRegistersShipmentDuringShift) {
    clerk.startShift();
    clerk.acceptShipment(branch, letter);
    CHECK_EQUAL(std::size_t(1), clerk.acceptedCount());
    CHECK_EQUAL(std::size_t(1), branch.shipmentCount());
    CHECK_EQUAL(letter.trackingNumber(), branch.findShipment("LT001").trackingNumber());
}

TEST_FIXTURE(StaffFixture, ClerkCannotWorkOffShiftOrAtWrongBranch) {
    CHECK_THROW(clerk.acceptShipment(branch, letter), std::logic_error);
    clerk.startShift();
    CHECK_THROW(clerk.acceptShipment(otherBranch, letter), std::logic_error);
    CHECK_EQUAL(std::size_t(0), clerk.acceptedCount());
    CHECK_EQUAL(std::size_t(0), branch.shipmentCount());
    CHECK_EQUAL(std::size_t(0), otherBranch.shipmentCount());
}

TEST_FIXTURE(StaffFixture, ClerkDoesNotCountRejectedShipment) {
    clerk.startShift();
    clerk.acceptShipment(branch, letter);
    CHECK_THROW(clerk.acceptShipment(branch, letter), DuplicateShipmentException);
    CHECK_EQUAL(std::size_t(1), clerk.acceptedCount());
    Letter dispatched("LT002", sender, recipient, 20, 5);
    dispatched.dispatch();
    CHECK_THROW(clerk.acceptShipment(branch, dispatched), ShipmentStateException);
    CHECK_EQUAL(std::size_t(1), clerk.acceptedCount());
}

TEST_FIXTURE(StaffFixture, CourierDispatchesAndDeliversRegisteredShipment) {
    branch.acceptShipment(letter);
    courier.startShift();
    courier.dispatchShipment(branch, "LT001");
    CHECK(branch.findShipment("LT001").status() == ShipmentStatus::InTransit);
    CHECK_EQUAL(std::size_t(1), courier.dispatchedCount());
    courier.deliverShipment(branch, "LT001");
    CHECK(branch.findShipment("LT001").status() == ShipmentStatus::Delivered);
    CHECK_EQUAL(std::size_t(1), courier.deliveredCount());
}

TEST_FIXTURE(StaffFixture, CourierCannotWorkOffShiftOrAtWrongBranch) {
    branch.acceptShipment(letter);
    otherBranch.acceptShipment(letter);
    CHECK_THROW(courier.dispatchShipment(branch, "LT001"), std::logic_error);
    CHECK_THROW(courier.deliverShipment(branch, "LT001"), std::logic_error);
    courier.startShift();
    CHECK_THROW(courier.dispatchShipment(otherBranch, "LT001"), std::logic_error);
    CHECK_THROW(courier.deliverShipment(otherBranch, "LT001"), std::logic_error);
    CHECK_EQUAL(std::size_t(0), courier.dispatchedCount());
    CHECK_EQUAL(std::size_t(0), courier.deliveredCount());
    CHECK(otherBranch.findShipment("LT001").status() == ShipmentStatus::Accepted);
}

TEST_FIXTURE(StaffFixture, CourierDoesNotCountFailedOperations) {
    branch.acceptShipment(letter);
    courier.startShift();
    CHECK_THROW(courier.dispatchShipment(branch, "missing"), ShipmentNotFoundException);
    CHECK_THROW(courier.deliverShipment(branch, "missing"), ShipmentNotFoundException);
    CHECK_THROW(courier.deliverShipment(branch, "LT001"), ShipmentStateException);
    CHECK_EQUAL(std::size_t(0), courier.dispatchedCount());
    CHECK_EQUAL(std::size_t(0), courier.deliveredCount());
    courier.dispatchShipment(branch, "LT001");
    CHECK_THROW(courier.dispatchShipment(branch, "LT001"), ShipmentStateException);
    courier.deliverShipment(branch, "LT001");
    CHECK_THROW(courier.deliverShipment(branch, "LT001"), ShipmentStateException);
    CHECK_EQUAL(std::size_t(1), courier.dispatchedCount());
    CHECK_EQUAL(std::size_t(1), courier.deliveredCount());
}

TEST_FIXTURE(StaffFixture, EmployeeRolesArePolymorphic) {
    const Employee &operatorRole = clerk;
    const Employee &courierRole = courier;
    CHECK_EQUAL(std::string("Postal clerk"), operatorRole.role());
    CHECK_EQUAL(std::string("Courier"), courierRole.role());
}

TEST_FIXTURE(StaffFixture, TransferredEmployeeWorksAtNewBranch) {
    clerk.assignToBranch(14);
    clerk.startShift();
    clerk.acceptShipment(otherBranch, letter);
    CHECK_EQUAL(std::size_t(1), otherBranch.shipmentCount());
    CHECK_EQUAL(std::size_t(0), branch.shipmentCount());
}
