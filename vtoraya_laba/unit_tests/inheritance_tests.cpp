#include "accessdeniedexception.h"
#include <UnitTest++/UnitTest++.h>
#include "universalemployee.h"
#include "cashier.h"
#include "branchmanager.h"
#include "letter.h"
#include <stdexcept>
#include <string>
#include <type_traits>

static_assert(!std::is_assignable<Employee &, const Employee &>::value, "Base assignment must not corrupt employee subtypes");
static_assert(std::is_convertible<UniversalEmployee *, Employee *>::value, "Diamond must have one accessible Employee");
static_assert(!std::is_convertible<Cashier *, CashRegister *>::value, "Private register must stay hidden");
static_assert(!std::is_convertible<BranchManager *, AccessPolicy *>::value, "Protected policy must stay hidden");

struct InheritanceFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    PostalBranch branch{13, address};
    PostalBranch otherBranch{14, address};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    UniversalEmployee universal{10, "Ольга", 13};
    Cashier cashier{11, "Иван", 13, 100};
    BranchManager manager{12, "Анна", 13};
};

TEST_FIXTURE(InheritanceFixture, VirtualDiamondSharesIdentityAndShift) {
    PostalClerk &clerk = universal;
    Courier &courier = universal;
    Employee *throughClerk = &clerk;
    Employee *throughCourier = &courier;
    CHECK(throughClerk == throughCourier);
    CHECK_EQUAL(10, universal.id());
    CHECK_EQUAL(std::string("Ольга"), universal.name());
    CHECK_EQUAL(std::string("Postal clerk and courier"), throughClerk->role());
    clerk.startShift();
    CHECK(courier.onShift());
    courier.endShift();
    CHECK(!clerk.onShift());
}

TEST_FIXTURE(InheritanceFixture, UniversalEmployeePerformsBothPostalRoles) {
    universal.startShift();
    universal.acceptShipment(branch, letter);
    universal.dispatchShipment(branch, "LT001");
    universal.deliverShipment(branch, "LT001");
    CHECK_EQUAL(std::size_t(1), universal.acceptedCount());
    CHECK_EQUAL(std::size_t(1), universal.dispatchedCount());
    CHECK_EQUAL(std::size_t(1), universal.deliveredCount());
    CHECK(branch.findShipment("LT001").status() == ShipmentStatus::Delivered);
}

TEST_FIXTURE(InheritanceFixture, CashierResolvesIdentityAndUsesPrivateRegister) {
    CHECK_EQUAL(11, cashier.id());
    CHECK_EQUAL(13, cashier.branchId());
    CHECK_EQUAL(std::string("Cashier"), cashier.role());
    CHECK_EQUAL(0, cashier.balance());
    CHECK_THROW(cashier.acceptPayment(branch, 1, letter), std::logic_error);
    Employee &employee = cashier;
    employee.startShift();
    Receipt receipt = cashier.acceptPayment(branch, 1, letter);
    CHECK_EQUAL(100, receipt.registerId());
    CHECK_EQUAL(100, cashier.balance());
    CHECK_EQUAL(100, cashier.findPayment(1).amount());
    employee.endShift();
    CHECK(!cashier.onShift());
    CHECK_THROW(cashier.acceptPayment(branch, 2, letter), std::logic_error);
}

TEST_FIXTURE(InheritanceFixture, CashierShiftErrorsPreserveWorkingState) {
    CHECK_THROW(cashier.endShift(), std::logic_error);
    cashier.startShift();
    CHECK_THROW(cashier.startShift(), std::logic_error);
    CHECK(cashier.onShift());
    cashier.acceptPayment(branch, 1, letter);
    cashier.endShift();
    CHECK_THROW(cashier.endShift(), std::logic_error);
    cashier.startShift();
    CHECK(cashier.onShift());
    CHECK_EQUAL(100, cashier.balance());
}

TEST_FIXTURE(InheritanceFixture, CashierChecksBranchAndProcessesRefunds) {
    cashier.startShift();
    CHECK_THROW(cashier.acceptPayment(otherBranch, 1, letter), std::logic_error);
    cashier.acceptPayment(branch, 1, letter);
    Refund refund(1, 1, 40, "reason");
    CHECK_THROW(cashier.processRefund(otherBranch, refund), std::logic_error);
    cashier.processRefund(branch, refund);
    CHECK_EQUAL(60, cashier.balance());
    CHECK_EQUAL(40, cashier.findRefund(1).amount());
    cashier.endShift();
    Refund remaining(2, 1, 60, "reason");
    CHECK_THROW(cashier.processRefund(branch, remaining), std::logic_error);
}

TEST_FIXTURE(InheritanceFixture, CashierCannotSeparateEmployeeAndRegisterBranches) {
    Employee &employee = cashier;
    CHECK_THROW(employee.assignToBranch(14), std::invalid_argument);
    CHECK_THROW(employee.assignToBranch(0), std::invalid_argument);
    employee.assignToBranch(13);
    cashier.startShift();
    CHECK_THROW(employee.assignToBranch(13), std::logic_error);
    CHECK_EQUAL(13, cashier.branchId());
}

TEST(AccessPolicyGrantsChecksAndRevokesPermissions) {
    AccessPolicy policy;
    CHECK(policy.enabled());
    CHECK_EQUAL(std::size_t(0), policy.deniedCount());
    CHECK(!policy.hasPermission("assign"));
    CHECK_THROW(policy.grantPermission(""), std::invalid_argument);
    CHECK_THROW(policy.grantPermission(" \t"), std::invalid_argument);
    policy.grantPermission("assign");
    policy.grantPermission("assign");
    CHECK(policy.hasPermission("assign"));
    policy.checkPermission("assign");
    policy.revokePermission("assign");
    CHECK(!policy.hasPermission("assign"));
    CHECK_THROW(policy.revokePermission("assign"), std::invalid_argument);
    CHECK_THROW(policy.checkPermission("assign"), AccessDeniedException);
    CHECK_EQUAL(std::size_t(1), policy.deniedCount());
}

TEST(AccessPolicyCanBeSuspendedWithoutLosingPermissions) {
    AccessPolicy policy;
    policy.grantPermission("shift");
    CHECK_THROW(policy.enable(), std::logic_error);
    policy.disable();
    CHECK(!policy.enabled());
    CHECK_THROW(policy.disable(), std::logic_error);
    CHECK_THROW(policy.checkPermission("shift"), AccessDeniedException);
    CHECK(policy.hasPermission("shift"));
    policy.enable();
    policy.checkPermission("shift");
    CHECK_EQUAL(std::size_t(1), policy.deniedCount());
}

TEST_FIXTURE(InheritanceFixture, ManagerAssignsEmployeesAndControlsBothCashierShifts) {
    CHECK_EQUAL(std::string("Branch manager"), manager.role());
    CHECK(manager.enabled());
    manager.startShift();
    PostalClerk other(20, "Павел", 14);
    manager.assignEmployee(branch, other);
    CHECK_EQUAL(13, other.branchId());
    CHECK_EQUAL(std::size_t(1), manager.assignedCount());
    manager.startEmployeeShift(branch, cashier);
    cashier.acceptPayment(branch, 1, letter);
    manager.endEmployeeShift(branch, cashier);
    CHECK(!cashier.onShift());
    CHECK_EQUAL(std::size_t(1), manager.startedShifts());
    CHECK_EQUAL(std::size_t(1), manager.endedShifts());
    CHECK_THROW(cashier.acceptPayment(branch, 2, letter), std::logic_error);
}

TEST_FIXTURE(InheritanceFixture, ManagerRequiresOwnShiftCorrectBranchAndPermission) {
    CHECK_THROW(manager.assignEmployee(branch, universal), std::logic_error);
    manager.startShift();
    CHECK_THROW(manager.assignEmployee(otherBranch, universal), std::logic_error);
    manager.suspendManagement();
    CHECK(!manager.enabled());
    CHECK_THROW(manager.assignEmployee(branch, universal), AccessDeniedException);
    CHECK_EQUAL(std::size_t(1), manager.deniedCount());
    manager.restoreManagement();
    manager.assignEmployee(branch, universal);
    CHECK_EQUAL(std::size_t(1), manager.assignedCount());
}

TEST_FIXTURE(InheritanceFixture, ManagerRejectsWrongEmployeeBranchAndOwnShiftManagement) {
    manager.startShift();
    PostalClerk other(20, "Павел", 14);
    CHECK_THROW(manager.startEmployeeShift(branch, other), std::invalid_argument);
    CHECK_THROW(manager.endEmployeeShift(branch, other), std::invalid_argument);
    CHECK_THROW(manager.startEmployeeShift(branch, manager), std::logic_error);
    CHECK_THROW(manager.assignEmployee(branch, manager), std::logic_error);
    CHECK_EQUAL(std::size_t(0), manager.assignedCount());
    CHECK_EQUAL(std::size_t(0), manager.startedShifts());
    CHECK_EQUAL(std::size_t(0), manager.endedShifts());
}

TEST_FIXTURE(InheritanceFixture, ManagerCountsOnlySuccessfulOperations) {
    manager.startShift();
    universal.startShift();
    CHECK_THROW(manager.assignEmployee(branch, universal), std::logic_error);
    CHECK_THROW(manager.startEmployeeShift(branch, universal), std::logic_error);
    manager.endEmployeeShift(branch, universal);
    CHECK_THROW(manager.endEmployeeShift(branch, universal), std::logic_error);
    CHECK_EQUAL(std::size_t(0), manager.assignedCount());
    CHECK_EQUAL(std::size_t(0), manager.startedShifts());
    CHECK_EQUAL(std::size_t(1), manager.endedShifts());
}
