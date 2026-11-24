#include "capacityexceededexception.h"
#include <UnitTest++/UnitTest++.h>
#include "vehicle.h"
#include "letter.h"
#include "parcel.h"
#include <limits>
#include <stdexcept>
#include <string>

struct LogisticsFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    PostalBranch first{13, address};
    PostalBranch second{14, address};
    PostalBranch third{15, address};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    Parcel parcel{"PK001", sender, recipient, 1500, 200, 100};
    Route route{1, "Минский маршрут"};
    MailBag bag{"BAG001", 13, 15, 2000};
    LogisticsFixture() {
        route.addStop(RouteStop(first, 0, 0));
        route.addStop(RouteStop(second, 10, 20));
        route.addStop(RouteStop(third, 20, 40));
    }
};

TEST_FIXTURE(LogisticsFixture, RouteStopStoresBranchAddressDistanceAndSchedule) {
    RouteStop stop(second, 10, 20);
    CHECK_EQUAL(14, stop.branchId());
    CHECK_EQUAL(address.formatLabel(), stop.address().formatLabel());
    CHECK_EQUAL(10, stop.distanceKm());
    CHECK_EQUAL(20, stop.plannedMinute());
    CHECK(stop.describe().find("Branch 14") != std::string::npos);
    CHECK_THROW(RouteStop(second, -1, 20), std::invalid_argument);
    CHECK_THROW(RouteStop(second, 10, -1), std::invalid_argument);
}

TEST(RouteRejectsInvalidIdentity) {
    CHECK_THROW(Route(0, "route"), std::invalid_argument);
    CHECK_THROW(Route(-1, "route"), std::invalid_argument);
    CHECK_THROW(Route(1, ""), std::invalid_argument);
    CHECK_THROW(Route(1, " \t\n"), std::invalid_argument);
}

TEST_FIXTURE(LogisticsFixture, RouteQueriesDistanceStopsAndRemainingDestinations) {
    CHECK_EQUAL(1, route.id());
    CHECK_EQUAL(std::string("Минский маршрут"), route.name());
    CHECK_EQUAL(std::size_t(3), route.stopCount());
    CHECK_EQUAL(30, route.totalDistanceKm());
    CHECK_EQUAL(14, route.stopAt(1).branchId());
    CHECK(route.canReachAfter(0, 15));
    CHECK(!route.canReachAfter(1, 13));
    CHECK(!route.canReachAfter(2, 15));
    CHECK(!route.canReachAfter(0, 99));
    CHECK_THROW(route.stopAt(3), std::out_of_range);
    CHECK_THROW(route.canReachAfter(3, 15), std::out_of_range);
}

TEST_FIXTURE(LogisticsFixture, RouteRejectsInconsistentStopSequence) {
    Route empty(2, "other");
    CHECK_THROW(empty.addStop(RouteStop(first, 1, 0)), std::invalid_argument);
    CHECK_EQUAL(std::size_t(0), empty.stopCount());
    CHECK_THROW(route.addStop(RouteStop(third, 1, 50)), std::invalid_argument);
    CHECK_THROW(route.addStop(RouteStop(first, 1, 40)), std::invalid_argument);
    CHECK_THROW(route.addStop(RouteStop(first, 1, 39)), std::invalid_argument);
    CHECK_EQUAL(std::size_t(3), route.stopCount());
}

TEST_FIXTURE(LogisticsFixture, RouteChecksTotalDistanceOverflow) {
    Route longRoute(2, "long");
    longRoute.addStop(RouteStop(first, 0, 0));
    longRoute.addStop(RouteStop(second, std::numeric_limits<int>::max(), 1));
    CHECK_THROW(longRoute.addStop(RouteStop(third, 1, 2)), std::overflow_error);
    CHECK_EQUAL(std::numeric_limits<int>::max(), longRoute.totalDistanceKm());
    CHECK_EQUAL(std::size_t(2), longRoute.stopCount());
}

TEST_FIXTURE(LogisticsFixture, MailBagStoresDestinationAndStartsEmpty) {
    CHECK_EQUAL(std::string("BAG001"), bag.id());
    CHECK_EQUAL(13, bag.sourceBranchId());
    CHECK_EQUAL(15, bag.destinationBranchId());
    CHECK_EQUAL(2000, bag.maximumWeight());
    CHECK_EQUAL(0, bag.weightGrams());
    CHECK_EQUAL(2000, bag.remainingCapacity());
    CHECK_EQUAL(std::size_t(0), bag.shipmentCount());
    CHECK(!bag.sealed());
    CHECK(!bag.contains("LT001"));
}

TEST(MailBagRejectsInvalidIdentityDestinationAndCapacity) {
    CHECK_THROW(MailBag("", 13, 15, 2000), std::invalid_argument);
    CHECK_THROW(MailBag("BAG 1", 13, 15, 2000), std::invalid_argument);
    CHECK_THROW(MailBag("BAG1", 0, 15, 2000), std::invalid_argument);
    CHECK_THROW(MailBag("BAG1", 13, 0, 2000), std::invalid_argument);
    CHECK_THROW(MailBag("BAG1", 13, 13, 2000), std::invalid_argument);
    CHECK_THROW(MailBag("BAG1", 13, 15, 0), std::invalid_argument);
}

TEST_FIXTURE(LogisticsFixture, MailBagCountsPolymorphicShipmentWeights) {
    bag.addShipment(letter);
    parcel.dispatch();
    bag.addShipment(parcel);
    CHECK_EQUAL(1520, bag.weightGrams());
    CHECK_EQUAL(480, bag.remainingCapacity());
    CHECK_EQUAL(std::size_t(2), bag.shipmentCount());
    CHECK(bag.contains("LT001"));
    CHECK_EQUAL(std::string("PK001"), bag.trackingNumberAt(1));
    CHECK_THROW(bag.trackingNumberAt(2), std::out_of_range);
}

TEST_FIXTURE(LogisticsFixture, MailBagRejectsDuplicateOverweightAndDeliveredShipment) {
    MailBag tiny("TINY", 13, 15, 20);
    tiny.addShipment(letter);
    CHECK_EQUAL(0, tiny.remainingCapacity());
    CHECK_THROW(tiny.addShipment(letter), std::invalid_argument);
    CHECK_THROW(tiny.addShipment(parcel), CapacityExceededException);
    CHECK_EQUAL(20, tiny.weightGrams());
    parcel.dispatch();
    parcel.deliver();
    CHECK_THROW(bag.addShipment(parcel), std::logic_error);
    CHECK_EQUAL(0, bag.weightGrams());
}

TEST_FIXTURE(LogisticsFixture, MailBagValidatesSealOpenAndClearSequence) {
    CHECK_THROW(bag.seal(), std::logic_error);
    CHECK_THROW(bag.open(), std::logic_error);
    bag.addShipment(letter);
    bag.seal();
    CHECK(bag.sealed());
    CHECK_THROW(bag.seal(), std::logic_error);
    CHECK_THROW(bag.clear(), std::logic_error);
    CHECK_THROW(bag.addShipment(parcel), std::logic_error);
    bag.open();
    bag.clear();
    CHECK(!bag.sealed());
    CHECK_EQUAL(0, bag.weightGrams());
    CHECK_EQUAL(std::size_t(0), bag.shipmentCount());
}

TEST_FIXTURE(LogisticsFixture, VehicleValidatesPlateCapacityAndRoute) {
    CHECK_THROW(Vehicle("", 3000, route), std::invalid_argument);
    CHECK_THROW(Vehicle(" \t", 3000, route), std::invalid_argument);
    CHECK_THROW(Vehicle("1234 AB-7", 0, route), std::invalid_argument);
    Route empty(2, "empty");
    CHECK_THROW(Vehicle("1234 AB-7", 3000, empty), std::invalid_argument);
    empty.addStop(RouteStop(first, 0, 0));
    CHECK_THROW(Vehicle("1234 AB-7", 3000, empty), std::invalid_argument);
}

TEST_FIXTURE(LogisticsFixture, VehicleCarriesBagThroughIntermediateStop) {
    Vehicle vehicle("1234 AB-7", 3000, route);
    CHECK_EQUAL(std::string("1234 AB-7"), vehicle.plate());
    CHECK_EQUAL(3000, vehicle.maximumLoad());
    CHECK_EQUAL(13, vehicle.currentBranchId());
    CHECK(!vehicle.moving());
    bag.addShipment(letter);
    bag.addShipment(parcel);
    bag.seal();
    vehicle.loadBag(bag);
    CHECK_EQUAL(1520, vehicle.loadGrams());
    CHECK_EQUAL(1480, vehicle.remainingCapacity());
    vehicle.depart();
    CHECK(vehicle.moving());
    vehicle.arriveAt(14);
    vehicle.depart();
    vehicle.arriveAt(15);
    MailBag unloaded = vehicle.unloadBag("BAG001");
    CHECK_EQUAL(15, vehicle.currentBranchId());
    CHECK_EQUAL(std::string("BAG001"), unloaded.id());
    CHECK(unloaded.sealed());
    CHECK_EQUAL(std::size_t(2), unloaded.shipmentCount());
    CHECK_EQUAL(std::size_t(0), vehicle.bagCount());
    CHECK_EQUAL(0, vehicle.loadGrams());
    CHECK_THROW(vehicle.depart(), std::logic_error);
}

TEST_FIXTURE(LogisticsFixture, VehicleRejectsUnsealedWrongSourceAndUnreachableDestination) {
    Vehicle vehicle("1234 AB-7", 3000, route);
    bag.addShipment(letter);
    CHECK_THROW(vehicle.loadBag(bag), std::logic_error);
    MailBag wrongSource("WRONG", 14, 15, 2000);
    wrongSource.addShipment(letter);
    wrongSource.seal();
    CHECK_THROW(vehicle.loadBag(wrongSource), std::invalid_argument);
    MailBag unreachable("FAR", 13, 99, 2000);
    unreachable.addShipment(letter);
    unreachable.seal();
    CHECK_THROW(vehicle.loadBag(unreachable), std::invalid_argument);
    CHECK_EQUAL(std::size_t(0), vehicle.bagCount());
}

TEST_FIXTURE(LogisticsFixture, VehicleChecksRemainingCapacity) {
    Vehicle vehicle("1234 AB-7", 1500, route);
    bag.addShipment(parcel);
    bag.seal();
    vehicle.loadBag(bag);
    CHECK_EQUAL(0, vehicle.remainingCapacity());
    MailBag additional("BAG002", 13, 15, 2000);
    additional.addShipment(letter);
    additional.seal();
    CHECK_THROW(vehicle.loadBag(additional), CapacityExceededException);
    CHECK_EQUAL(1500, vehicle.loadGrams());
}

TEST_FIXTURE(LogisticsFixture, VehicleRejectsDuplicateBagsAndShipments) {
    Vehicle vehicle("1234 AB-7", 5000, route);
    bag.addShipment(letter);
    bag.seal();
    vehicle.loadBag(bag);
    CHECK_THROW(vehicle.loadBag(bag), std::invalid_argument);
    MailBag duplicate("BAG002", 13, 15, 2000);
    duplicate.addShipment(letter);
    duplicate.seal();
    CHECK_THROW(vehicle.loadBag(duplicate), std::invalid_argument);
    CHECK_EQUAL(std::size_t(1), vehicle.bagCount());
}

TEST_FIXTURE(LogisticsFixture, VehicleRequiresDepartureAndNextStopForArrival) {
    Vehicle vehicle("1234 AB-7", 3000, route);
    CHECK_THROW(vehicle.arriveAt(14), std::logic_error);
    vehicle.depart();
    CHECK_THROW(vehicle.depart(), std::logic_error);
    CHECK_THROW(vehicle.arriveAt(15), std::invalid_argument);
    CHECK(vehicle.moving());
    CHECK_EQUAL(13, vehicle.currentBranchId());
    vehicle.arriveAt(14);
    CHECK(!vehicle.moving());
}

TEST_FIXTURE(LogisticsFixture, VehicleRejectsLoadingAndUnloadingWhileMoving) {
    Vehicle vehicle("1234 AB-7", 3000, route);
    bag.addShipment(letter);
    bag.seal();
    vehicle.loadBag(bag);
    vehicle.depart();
    CHECK_THROW(vehicle.loadBag(bag), std::logic_error);
    CHECK_THROW(vehicle.unloadBag("BAG001"), std::logic_error);
    CHECK_EQUAL(std::size_t(1), vehicle.bagCount());
}

TEST_FIXTURE(LogisticsFixture, VehicleRejectsEarlyUnloadAndUnknownBag) {
    Vehicle vehicle("1234 AB-7", 3000, route);
    bag.addShipment(letter);
    bag.seal();
    vehicle.loadBag(bag);
    CHECK_THROW(vehicle.unloadBag("BAG001"), std::logic_error);
    CHECK_THROW(vehicle.unloadBag("missing"), std::out_of_range);
    CHECK_EQUAL(std::size_t(1), vehicle.bagCount());
}

TEST_FIXTURE(LogisticsFixture, VehicleRequiresUnloadingAtIntermediateDestination) {
    Vehicle vehicle("1234 AB-7", 3000, route);
    MailBag local("LOCAL", 13, 14, 2000);
    local.addShipment(letter);
    local.seal();
    vehicle.loadBag(local);
    vehicle.depart();
    vehicle.arriveAt(14);
    CHECK_THROW(vehicle.depart(), std::logic_error);
    vehicle.unloadBag("LOCAL");
    vehicle.depart();
    vehicle.arriveAt(15);
    CHECK_EQUAL(15, vehicle.currentBranchId());
}

TEST_FIXTURE(LogisticsFixture, VehicleOwnsLoadedBagAndRouteSnapshots) {
    Vehicle vehicle("1234 AB-7", 3000, route);
    bag.addShipment(letter);
    bag.seal();
    vehicle.loadBag(bag);
    bag.open();
    bag.clear();
    route.addStop(RouteStop(first, 10, 60));
    CHECK_EQUAL(20, vehicle.loadGrams());
    vehicle.depart();
    vehicle.arriveAt(14);
    vehicle.depart();
    vehicle.arriveAt(15);
    MailBag stored = vehicle.unloadBag("BAG001");
    CHECK_EQUAL(20, stored.weightGrams());
    CHECK_THROW(vehicle.depart(), std::logic_error);
}

TEST_FIXTURE(LogisticsFixture, VehicleCanLoadAtAnIntermediateSource) {
    Vehicle vehicle("1234 AB-7", 3000, route);
    vehicle.depart();
    vehicle.arriveAt(14);
    MailBag local("LOCAL", 14, 15, 2000);
    local.addShipment(letter);
    local.seal();
    vehicle.loadBag(local);
    vehicle.depart();
    vehicle.arriveAt(15);
    MailBag unloaded = vehicle.unloadBag("LOCAL");
    CHECK_EQUAL(std::string("LOCAL"), unloaded.id());
}
