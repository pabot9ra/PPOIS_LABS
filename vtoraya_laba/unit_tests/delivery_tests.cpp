#include "shipmentstateexception.h"
#include <UnitTest++/UnitTest++.h>
#include "postalbranch.h"
#include "shipmentnotfoundexception.h"
#include "letter.h"
#include "parcel.h"
#include <stdexcept>

struct BranchDeliveryFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    Parcel parcel{"PK001", sender, recipient, 1500, 200, 100};
    PostalBranch branch{13, address};
    BranchDeliveryFixture() {
        branch.acceptShipment(letter);
        branch.acceptShipment(parcel);
    }
};

TEST_FIXTURE(BranchDeliveryFixture, BranchDispatchesOnlySelectedShipment) {
    const Shipment &stored = branch.findShipment("LT001");
    branch.dispatchShipment("LT001");
    CHECK(stored.status() == ShipmentStatus::InTransit);
    CHECK(branch.findShipment("PK001").status() == ShipmentStatus::Accepted);
    CHECK(letter.status() == ShipmentStatus::Accepted);
    CHECK_EQUAL(std::size_t(2), branch.shipmentCount());
}

TEST_FIXTURE(BranchDeliveryFixture, BranchDeliversLetterAndParcel) {
    branch.dispatchShipment("LT001");
    branch.deliverShipment("LT001");
    branch.dispatchShipment("PK001");
    branch.deliverShipment("PK001");
    CHECK(branch.findShipment("LT001").status() == ShipmentStatus::Delivered);
    CHECK(branch.findShipment("PK001").status() == ShipmentStatus::Delivered);
    CHECK_EQUAL(100, branch.findShipment("LT001").calculatePrice());
    CHECK_EQUAL(400, branch.findShipment("PK001").calculatePrice());
    CHECK(dynamic_cast<const Letter *>(&branch.findShipment("LT001")) != nullptr);
    CHECK(dynamic_cast<const Parcel *>(&branch.findShipment("PK001")) != nullptr);
}

TEST_FIXTURE(BranchDeliveryFixture, BranchCannotDeliverBeforeDispatch) {
    CHECK_THROW(branch.deliverShipment("LT001"), ShipmentStateException);
    CHECK(branch.findShipment("LT001").status() == ShipmentStatus::Accepted);
}

TEST_FIXTURE(BranchDeliveryFixture, BranchCannotDispatchTwice) {
    branch.dispatchShipment("LT001");
    CHECK_THROW(branch.dispatchShipment("LT001"), ShipmentStateException);
    CHECK(branch.findShipment("LT001").status() == ShipmentStatus::InTransit);
}

TEST_FIXTURE(BranchDeliveryFixture, BranchCannotChangeDeliveredShipment) {
    branch.dispatchShipment("PK001");
    branch.deliverShipment("PK001");
    CHECK_THROW(branch.dispatchShipment("PK001"), ShipmentStateException);
    CHECK_THROW(branch.deliverShipment("PK001"), ShipmentStateException);
    CHECK(branch.findShipment("PK001").status() == ShipmentStatus::Delivered);
}

TEST_FIXTURE(BranchDeliveryFixture, BranchReportsMissingTrackingNumberWithoutChanges) {
    CHECK_THROW(branch.dispatchShipment("missing"), ShipmentNotFoundException);
    CHECK_THROW(branch.deliverShipment("missing"), ShipmentNotFoundException);
    CHECK(branch.findShipment("LT001").status() == ShipmentStatus::Accepted);
    CHECK(branch.findShipment("PK001").status() == ShipmentStatus::Accepted);
    CHECK_EQUAL(std::size_t(2), branch.shipmentCount());
}

TEST(EmptyBranchCannotDispatchOrDeliver) {
    PostalAddress address("Минск", "Гикало", "9", "220013");
    PostalBranch branch(13, address);
    CHECK_THROW(branch.dispatchShipment("LT001"), ShipmentNotFoundException);
    CHECK_THROW(branch.deliverShipment("LT001"), ShipmentNotFoundException);
    CHECK_EQUAL(std::size_t(0), branch.shipmentCount());
}
