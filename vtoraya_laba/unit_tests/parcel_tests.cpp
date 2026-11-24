#include <UnitTest++/UnitTest++.h>
#include "parcel.h"
#include "letter.h"
#include "postalbranch.h"
#include <limits>
#include <stdexcept>

struct ParcelFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Parcel parcel{"PK001", sender, recipient, 1500, 200, 100};
};

TEST_FIXTURE(ParcelFixture, ParcelStoresWeightAndTariff) {
    CHECK_EQUAL(1500, parcel.weightGrams());
    CHECK_EQUAL(200, parcel.basePrice());
    CHECK_EQUAL(100, parcel.pricePerKilogram());
    CHECK_EQUAL(2, parcel.chargedKilograms());
    CHECK_EQUAL(400, parcel.calculatePrice());
}

TEST_FIXTURE(ParcelFixture, ParcelRoundsStartedKilogramsUp) {
    const int weights[] = {1, 999, 1000, 1001, 2000, 2001};
    const int expected[] = {1, 1, 1, 2, 2, 3};
    for(int i = 0; i < 6; i++){
        Parcel item("PK002", sender, recipient, weights[i], 0, 100);
        CHECK_EQUAL(expected[i], item.chargedKilograms());
        CHECK_EQUAL(expected[i] * 100, item.calculatePrice());
    }
}

TEST_FIXTURE(ParcelFixture, ParcelRejectsInvalidWeightAndTariff) {
    CHECK_THROW(Parcel("PK002", sender, recipient, 0, 200, 100), std::invalid_argument);
    CHECK_THROW(Parcel("PK002", sender, recipient, -1, 200, 100), std::invalid_argument);
    CHECK_THROW(Parcel("PK002", sender, recipient, 1000, -1, 100), std::invalid_argument);
    CHECK_THROW(Parcel("PK002", sender, recipient, 1000, 200, 0), std::invalid_argument);
    CHECK_THROW(Parcel("PK002", sender, recipient, 1000, 200, -1), std::invalid_argument);
}

TEST_FIXTURE(ParcelFixture, ParcelRejectsOverflowAndAcceptsExactPriceLimit) {
    int maximum = std::numeric_limits<int>::max();
    CHECK_THROW(Parcel("PK002", sender, recipient, 1001, 0, maximum), std::overflow_error);
    CHECK_THROW(Parcel("PK002", sender, recipient, 1, 1, maximum), std::overflow_error);
    CHECK_THROW(Parcel("PK002", sender, recipient, 1, maximum, 1), std::overflow_error);
    Parcel limit("PK002", sender, recipient, 1000, 1, maximum - 1);
    CHECK_EQUAL(maximum, limit.calculatePrice());
}

TEST_FIXTURE(ParcelFixture, ParcelHandlesMaximumWeightWithoutOverflow) {
    Parcel heavy("PK002", sender, recipient, std::numeric_limits<int>::max(), 0, 1);
    CHECK_EQUAL(2147484, heavy.chargedKilograms());
    CHECK_EQUAL(2147484, heavy.calculatePrice());
}

TEST_FIXTURE(ParcelFixture, ParcelClonePreservesTypeAndStatus) {
    parcel.dispatch();
    auto copy = parcel.clone();
    CHECK(dynamic_cast<const Parcel *>(copy.get()) != nullptr);
    CHECK(copy->status() == ShipmentStatus::InTransit);
    CHECK_EQUAL(400, copy->calculatePrice());
    copy->deliver();
    CHECK(parcel.status() == ShipmentStatus::InTransit);
}

TEST_FIXTURE(ParcelFixture, BranchCalculatesDifferentShipmentPricesPolymorphically) {
    Letter letter("LT001", sender, recipient, 20, 5);
    PostalBranch branch(13, address);
    branch.acceptShipment(letter);
    branch.acceptShipment(parcel);
    const Shipment &storedLetter = branch.findShipment("LT001");
    const Shipment &storedParcel = branch.findShipment("PK001");
    CHECK_EQUAL(100, storedLetter.calculatePrice());
    CHECK_EQUAL(400, storedParcel.calculatePrice());
    CHECK(dynamic_cast<const Letter *>(&storedLetter) != nullptr);
    CHECK(dynamic_cast<const Parcel *>(&storedParcel) != nullptr);
    CHECK_EQUAL(std::size_t(2), branch.shipmentCount());
}
