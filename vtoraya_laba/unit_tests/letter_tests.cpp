#include <UnitTest++/UnitTest++.h>
#include "letter.h"
#include "postalbranch.h"
#include <limits>
#include <stdexcept>
#include <type_traits>

static_assert(std::is_abstract<Shipment>::value, "Shipment must be abstract");
static_assert(std::has_virtual_destructor<Shipment>::value, "Shipment destructor must be virtual");

struct LetterFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"BY001", sender, recipient, 20, 5};
};

TEST_FIXTURE(LetterFixture, LetterStoresWeightAndTariff) {
    CHECK_EQUAL(20, letter.weightGrams());
    CHECK_EQUAL(5, letter.pricePerGram());
    CHECK_EQUAL(100, letter.calculatePrice());
}

TEST_FIXTURE(LetterFixture, LetterPriceWorksThroughBaseReference) {
    const Shipment &shipment = letter;
    CHECK_EQUAL(100, shipment.calculatePrice());
}

TEST_FIXTURE(LetterFixture, LetterRejectsInvalidWeightAndTariff) {
    CHECK_THROW(Letter("BY002", sender, recipient, 0, 5), std::invalid_argument);
    CHECK_THROW(Letter("BY002", sender, recipient, -1, 5), std::invalid_argument);
    CHECK_THROW(Letter("BY002", sender, recipient, 20, 0), std::invalid_argument);
    CHECK_THROW(Letter("BY002", sender, recipient, 20, -1), std::invalid_argument);
}

TEST_FIXTURE(LetterFixture, LetterRejectsOverflowAndAcceptsExactLimit) {
    int maximum = std::numeric_limits<int>::max();
    CHECK_THROW(Letter("BY002", sender, recipient, 2, maximum), std::overflow_error);
    Letter limit("BY002", sender, recipient, 1, maximum);
    CHECK_EQUAL(maximum, limit.calculatePrice());
}

TEST_FIXTURE(LetterFixture, LetterClonePreservesTypeStateAndIndependence) {
    letter.dispatch();
    auto copy = letter.clone();
    CHECK(dynamic_cast<const Letter *>(copy.get()) != nullptr);
    CHECK(copy->status() == ShipmentStatus::InTransit);
    CHECK_EQUAL(letter.trackingNumber(), copy->trackingNumber());
    CHECK_EQUAL(100, copy->calculatePrice());
    copy->deliver();
    CHECK(letter.status() == ShipmentStatus::InTransit);
}

TEST_FIXTURE(LetterFixture, BranchPreservesLetterTypeAndPrice) {
    PostalBranch branch(13, address);
    branch.acceptShipment(letter);
    const Shipment &found = branch.findShipment("BY001");
    const Letter *storedLetter = dynamic_cast<const Letter *>(&found);
    CHECK(storedLetter != nullptr);
    CHECK_EQUAL(100, found.calculatePrice());
    letter.dispatch();
    CHECK(found.status() == ShipmentStatus::Accepted);
    for(int i = 1; i <= 20; i++){
        Letter extra("EXTRA" + std::to_string(i), sender, recipient, i, 5);
        branch.acceptShipment(extra);
    }
    CHECK_EQUAL(100, found.calculatePrice());
    CHECK_EQUAL(std::size_t(21), branch.shipmentCount());
}
