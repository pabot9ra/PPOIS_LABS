#include "duplicateshipmentexception.h"
#include "shipmentstateexception.h"
#include <UnitTest++/UnitTest++.h>
#include "postalbranch.h"
#include "shipmentnotfoundexception.h"
#include "letter.h"
#include <stdexcept>
#include <string>

struct BranchFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter first{"BY001", sender, recipient, 20, 5};
    Letter second{"BY002", sender, recipient, 30, 5};
    PostalBranch branch{13, address};
};

TEST_FIXTURE(BranchFixture, BranchStartsEmptyAndStoresAddress) {
    CHECK_EQUAL(13, branch.id());
    CHECK_EQUAL(address.formatLabel(), branch.address().formatLabel());
    CHECK_EQUAL(std::size_t(0), branch.shipmentCount());
}

TEST_FIXTURE(BranchFixture, BranchRejectsNonpositiveId) {
    CHECK_THROW(PostalBranch(0, address), std::invalid_argument);
    CHECK_THROW(PostalBranch(-1, address), std::invalid_argument);
}

TEST_FIXTURE(BranchFixture, BranchAcceptsAndFindsSeveralShipments) {
    branch.acceptShipment(first);
    branch.acceptShipment(second);
    CHECK_EQUAL(std::size_t(2), branch.shipmentCount());
    const Shipment &found = branch.findShipment("BY002");
    CHECK_EQUAL(std::string("BY002"), found.trackingNumber());
    CHECK_EQUAL(1, found.sender().id());
    CHECK_EQUAL(2, found.recipient().id());
    CHECK(found.status() == ShipmentStatus::Accepted);
}

TEST_FIXTURE(BranchFixture, BranchRejectsDuplicateWithoutAddingIt) {
    branch.acceptShipment(first);
    Letter duplicate("BY001", recipient, sender, 10, 5);
    CHECK_THROW(branch.acceptShipment(duplicate), DuplicateShipmentException);
    CHECK_EQUAL(std::size_t(1), branch.shipmentCount());
    const Shipment &original = branch.findShipment("BY001");
    CHECK_EQUAL(1, original.sender().id());
}

TEST_FIXTURE(BranchFixture, BranchRejectsInTransitAndDeliveredShipments) {
    first.dispatch();
    CHECK_THROW(branch.acceptShipment(first), ShipmentStateException);
    first.deliver();
    CHECK_THROW(branch.acceptShipment(first), ShipmentStateException);
    CHECK_EQUAL(std::size_t(0), branch.shipmentCount());
}

TEST_FIXTURE(BranchFixture, BranchReportsMissingShipmentWithoutChangingStorage) {
    CHECK_THROW(branch.findShipment("BY001"), ShipmentNotFoundException);
    branch.acceptShipment(first);
    CHECK_THROW(branch.findShipment("missing"), ShipmentNotFoundException);
    CHECK_EQUAL(std::size_t(1), branch.shipmentCount());
}

TEST_FIXTURE(BranchFixture, BranchOwnsRegisteredShipmentCopy) {
    branch.acceptShipment(first);
    first.dispatch();
    const Shipment &stored = branch.findShipment("BY001");
    CHECK(stored.status() == ShipmentStatus::Accepted);
}

TEST_FIXTURE(BranchFixture, BranchCloneReturnsIndependentCopy) {
    branch.acceptShipment(first);
    const Shipment &found = branch.findShipment("BY001");
    auto copy = found.clone();
    copy->dispatch();
    const Shipment &stored = branch.findShipment("BY001");
    CHECK(stored.status() == ShipmentStatus::Accepted);
}
