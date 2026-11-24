#include <UnitTest++/UnitTest++.h>
#include "shipmentnotfoundexception.h"
#include "postalbranch.h"
#include "letter.h"
#include <exception>
#include <string>

TEST(ShipmentNotFoundExceptionStoresTrackingNumberAndMessage) {
    ShipmentNotFoundException error("MISSING001");
    CHECK_EQUAL(std::string("MISSING001"), error.trackingNumber());
    CHECK_EQUAL(std::string("Shipment was not found at this branch: MISSING001"), std::string(error.what()));
}

TEST(PostalExceptionsCanBeCaughtByCommonBase) {
    CHECK_THROW(throw ShipmentNotFoundException("MISSING001"), PostalException);
    CHECK_THROW(throw ShipmentNotFoundException("MISSING001"), std::exception);
}

TEST(PostalExceptionPreservesMessage) {
    PostalException error("Postal operation failed");
    CHECK_EQUAL(std::string("Postal operation failed"), std::string(error.what()));
}

struct ExceptionBranchFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    PostalBranch branch{13, address};
    ExceptionBranchFixture() {
        branch.acceptShipment(letter);
    }
};

TEST_FIXTURE(ExceptionBranchFixture, FindFailureCarriesRequestedTrackingNumber) {
    bool caught = false;
    try {
        branch.findShipment("MISSING001");
    } catch(const ShipmentNotFoundException &error) {
        caught = true;
        CHECK_EQUAL(std::string("MISSING001"), error.trackingNumber());
    }
    CHECK(caught);
    CHECK_EQUAL(std::size_t(1), branch.shipmentCount());
}

TEST_FIXTURE(ExceptionBranchFixture, DispatchFailureCarriesRequestedTrackingNumber) {
    bool caught = false;
    try {
        branch.dispatchShipment("MISSING002");
    } catch(const ShipmentNotFoundException &error) {
        caught = true;
        CHECK_EQUAL(std::string("MISSING002"), error.trackingNumber());
    }
    CHECK(caught);
    CHECK(branch.findShipment("LT001").status() == ShipmentStatus::Accepted);
}

TEST_FIXTURE(ExceptionBranchFixture, DeliveryFailureCarriesRequestedTrackingNumber) {
    bool caught = false;
    try {
        branch.deliverShipment("MISSING003");
    } catch(const ShipmentNotFoundException &error) {
        caught = true;
        CHECK_EQUAL(std::string("MISSING003"), error.trackingNumber());
    }
    CHECK(caught);
    CHECK(branch.findShipment("LT001").status() == ShipmentStatus::Accepted);
}
