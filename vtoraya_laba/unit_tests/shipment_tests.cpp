#include "shipmentstateexception.h"
#include <UnitTest++/UnitTest++.h>
#include "letter.h"
#include <stdexcept>
#include <string>

struct ShipmentFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter shipment{"BY123456", sender, recipient, 20, 5};
};

TEST_FIXTURE(ShipmentFixture, ShipmentStoresParticipantsAndTrackingNumber) {
    CHECK_EQUAL(std::string("BY123456"), shipment.trackingNumber());
    CHECK_EQUAL(1, shipment.sender().id());
    CHECK_EQUAL(2, shipment.recipient().id());
    CHECK(shipment.status() == ShipmentStatus::Accepted);
}

TEST_FIXTURE(ShipmentFixture, ShipmentRejectsBlankOrSpacedTrackingNumber) {
    const std::string invalidNumbers[] = {"", "  ", "BY 123", "BY\t123", "BY123\n", " BY123"};
    for(const std::string &number : invalidNumbers){
        CHECK_THROW(Letter(number, sender, recipient, 20, 5), std::invalid_argument);
    }
}

TEST_FIXTURE(ShipmentFixture, ShipmentFollowsDeliverySequence) {
    shipment.dispatch();
    CHECK(shipment.status() == ShipmentStatus::InTransit);
    shipment.deliver();
    CHECK(shipment.status() == ShipmentStatus::Delivered);
}

TEST_FIXTURE(ShipmentFixture, ShipmentCannotSkipDispatch) {
    CHECK_THROW(shipment.deliver(), ShipmentStateException);
    CHECK(shipment.status() == ShipmentStatus::Accepted);
}

TEST_FIXTURE(ShipmentFixture, ShipmentCannotDispatchTwice) {
    shipment.dispatch();
    CHECK_THROW(shipment.dispatch(), ShipmentStateException);
    CHECK(shipment.status() == ShipmentStatus::InTransit);
}

TEST_FIXTURE(ShipmentFixture, ShipmentCannotChangeDeliveredStatus) {
    shipment.dispatch();
    shipment.deliver();
    CHECK_THROW(shipment.dispatch(), ShipmentStateException);
    CHECK_THROW(shipment.deliver(), ShipmentStateException);
    CHECK(shipment.status() == ShipmentStatus::Delivered);
}

TEST_FIXTURE(ShipmentFixture, ShipmentKeepsOriginalParticipantAddresses) {
    PostalAddress replacement("Брест", "Советская", "1", "224000");
    sender.changeAddress(replacement);
    recipient.changeAddress(replacement);
    CHECK_EQUAL(std::string("Минск"), shipment.sender().address().city());
    CHECK_EQUAL(std::string("Минск"), shipment.recipient().address().city());
}
