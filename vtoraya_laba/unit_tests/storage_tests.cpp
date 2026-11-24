#include "capacityexceededexception.h"
#include <UnitTest++/UnitTest++.h>
#include "sortingcenter.h"
#include "vehicle.h"
#include "letter.h"
#include "parcel.h"
#include <stdexcept>
#include <string>

struct StorageFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    PostalBranch branch{13, address};
    PostalBranch otherBranch{14, address};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    Parcel parcel{"PK001", sender, recipient, 1500, 200, 100};
    Dimensions size{100, 100, 100};
    StorageCell cell{"C1", 13, Dimensions(200,200,200), 2000};
    StorageZone zone{"A", 13};
    StorageInventory inventory{branch, 3000};
    SortingCenter center{1, branch, 3000};
    StorageFixture() {
        zone.addCell(cell);
        zone.addCell(StorageCell("C2", 13, Dimensions(200,200,200), 2000));
    }
};

TEST_FIXTURE(StorageFixture, StorageCellStoresConfigurationAndValidatesIdentity) {
    CHECK_EQUAL(std::string("C1"), cell.id());
    CHECK_EQUAL(13, cell.branchId());
    CHECK_EQUAL(200, cell.dimensions().length());
    CHECK_EQUAL(2000, cell.maximumWeight());
    CHECK(!cell.occupied());
    CHECK_EQUAL(0, cell.weightGrams());
    CHECK_THROW(StorageCell("", 13, size, 2000), std::invalid_argument);
    CHECK_THROW(StorageCell("C 1", 13, size, 2000), std::invalid_argument);
    CHECK_THROW(StorageCell("C1", 0, size, 2000), std::invalid_argument);
    CHECK_THROW(StorageCell("C1", 13, size, 0), std::invalid_argument);
}

TEST_FIXTURE(StorageFixture, StorageCellStoresAndReleasesShipment) {
    CHECK_THROW(cell.release(), std::logic_error);
    CHECK(cell.canFit(letter, size));
    cell.store(letter, size);
    CHECK(cell.occupied());
    CHECK_EQUAL(std::string("LT001"), cell.trackingNumber());
    CHECK_EQUAL(20, cell.weightGrams());
    CHECK(!cell.canFit(parcel, size));
    CHECK_THROW(cell.store(parcel, size), std::logic_error);
    cell.release();
    CHECK(!cell.occupied());
    CHECK_EQUAL(0, cell.weightGrams());
}

TEST_FIXTURE(StorageFixture, StorageCellRejectsSizeWeightAndDeliveredShipment) {
    StorageCell tiny("T", 13, size, 20);
    CHECK_THROW(tiny.store(parcel, size), std::invalid_argument);
    CHECK_THROW(tiny.store(letter, Dimensions(101,101,101)), std::invalid_argument);
    letter.dispatch();
    letter.deliver();
    CHECK_THROW(tiny.store(letter, size), std::logic_error);
    CHECK(!tiny.occupied());
}

TEST_FIXTURE(StorageFixture, StorageZoneAddsOnlyUniqueEmptyCellsOfSameBranch) {
    CHECK_EQUAL(std::string("A"), zone.id());
    CHECK_EQUAL(13, zone.branchId());
    CHECK_EQUAL(std::size_t(2), zone.cellCount());
    CHECK_EQUAL(std::size_t(0), zone.occupiedCount());
    CHECK_THROW(StorageZone("", 13), std::invalid_argument);
    CHECK_THROW(StorageZone("A B", 13), std::invalid_argument);
    CHECK_THROW(StorageZone("A", 0), std::invalid_argument);
    CHECK_THROW(zone.addCell(cell), std::invalid_argument);
    CHECK_THROW(zone.addCell(StorageCell("C3",14,size,2000)), std::invalid_argument);
    cell.store(letter,size);
    CHECK_THROW(zone.addCell(cell), std::logic_error);
    CHECK_EQUAL(std::size_t(2), zone.cellCount());
}

TEST_FIXTURE(StorageFixture, StorageZoneFindsCellsAndTracksStoredShipments) {
    CHECK_EQUAL(std::string("C1"), zone.findAvailableCell(letter,size).id());
    zone.storeShipment(letter,size);
    CHECK(zone.contains("LT001"));
    CHECK(!zone.canStore(letter,size));
    CHECK_EQUAL(std::string("C1"), zone.findShipmentCell("LT001").id());
    CHECK_EQUAL(20, zone.findCell("C1").weightGrams());
    CHECK_EQUAL(20LL, zone.weightGrams());
    CHECK_EQUAL(std::size_t(1), zone.occupiedCount());
    CHECK_THROW(zone.storeShipment(letter,size), std::invalid_argument);
    CHECK_THROW(zone.findCell("missing"), std::out_of_range);
    CHECK_THROW(zone.findShipmentCell("missing"), std::out_of_range);
    zone.releaseShipment("LT001");
    CHECK_EQUAL(0LL, zone.weightGrams());
    CHECK_THROW(zone.releaseShipment("LT001"), std::out_of_range);
}

TEST_FIXTURE(StorageFixture, StorageZoneLockBlocksMutationsButPreservesRecords) {
    CHECK_THROW(zone.unlock(), std::logic_error);
    zone.storeShipment(letter,size);
    zone.lock();
    CHECK(zone.locked());
    CHECK_THROW(zone.lock(), std::logic_error);
    CHECK(!zone.canStore(parcel,size));
    CHECK_THROW(zone.findAvailableCell(parcel,size), std::logic_error);
    CHECK_THROW(zone.storeShipment(parcel,size), std::logic_error);
    CHECK_THROW(zone.releaseShipment("LT001"), std::logic_error);
    CHECK_THROW(zone.addCell(StorageCell("C3",13,size,2000)), std::logic_error);
    CHECK(zone.contains("LT001"));
    zone.unlock();
    zone.releaseShipment("LT001");
}

TEST_FIXTURE(StorageFixture, StorageZoneReportsLackOfSuitableCells) {
    StorageZone empty("EMPTY",13);
    CHECK(!empty.canStore(letter,size));
    CHECK_THROW(empty.findAvailableCell(letter,size), std::out_of_range);
    CHECK_THROW(empty.storeShipment(letter,size), std::out_of_range);
    zone.storeShipment(letter,size);
    zone.storeShipment(parcel,size);
    Letter third("LT003", sender, recipient, 20, 5);
    CHECK_THROW(zone.storeShipment(third,size), std::out_of_range);
    third.dispatch();
    third.deliver();
    CHECK_THROW(zone.storeShipment(third,size), std::logic_error);
}

TEST(StorageLocationValidatesFormatsAndComparesAddresses) {
    StorageLocation location(13,"A","C1");
    CHECK_EQUAL(13, location.branchId());
    CHECK_EQUAL(std::string("A"), location.zoneId());
    CHECK_EQUAL(std::string("C1"), location.cellId());
    CHECK_EQUAL(std::string("13/A/C1"), location.format());
    CHECK(location == StorageLocation(13,"A","C1"));
    CHECK(!(location == StorageLocation(14,"A","C1")));
    CHECK(!(location == StorageLocation(13,"B","C1")));
    CHECK(!(location == StorageLocation(13,"A","C2")));
    CHECK_THROW(StorageLocation(0,"A","C1"), std::invalid_argument);
    CHECK_THROW(StorageLocation(13,"","C1"), std::invalid_argument);
    CHECK_THROW(StorageLocation(13,"A B","C1"), std::invalid_argument);
    CHECK_THROW(StorageLocation(13,"A",""), std::invalid_argument);
}

TEST_FIXTURE(StorageFixture, StorageInventoryAcceptsOnlyEmptyUniqueZonesOfSameBranch) {
    CHECK_THROW(StorageInventory(branch,0), std::invalid_argument);
    CHECK_EQUAL(13, inventory.branchId());
    CHECK_EQUAL(std::size_t(0), inventory.zoneCount());
    inventory.addZone(zone);
    CHECK_EQUAL(std::size_t(1), inventory.zoneCount());
    CHECK_THROW(inventory.addZone(zone), std::invalid_argument);
    CHECK_THROW(inventory.addZone(StorageZone("OTHER",14)), std::invalid_argument);
    StorageZone occupied("O",13);
    occupied.addCell(cell);
    occupied.storeShipment(letter,size);
    CHECK_THROW(inventory.addZone(occupied), std::logic_error);
}

TEST_FIXTURE(StorageFixture, StorageInventoryStoresLocatesAndReleasesWithoutDuplicateRecords) {
    inventory.addZone(zone);
    StorageLocation location = inventory.storeShipment(letter,size);
    CHECK(location == StorageLocation(13,"A","C1"));
    CHECK(inventory.locateShipment("LT001") == location);
    CHECK_EQUAL(20, inventory.storedWeightGrams("LT001"));
    CHECK_EQUAL(20LL, inventory.weightGrams());
    CHECK_EQUAL(2980LL, inventory.remainingCapacity());
    CHECK_EQUAL(std::size_t(1), inventory.shipmentCount());
    CHECK_THROW(inventory.storeShipment(letter,size), std::invalid_argument);
    inventory.releaseShipment("LT001");
    CHECK(!inventory.contains("LT001"));
    CHECK_EQUAL(0LL, inventory.weightGrams());
    CHECK_THROW(inventory.locateShipment("LT001"), std::out_of_range);
    CHECK_THROW(inventory.storedWeightGrams("LT001"), std::out_of_range);
    CHECK_THROW(inventory.releaseShipment("LT001"), std::out_of_range);
}

TEST_FIXTURE(StorageFixture, StorageInventoryEnforcesOverallWeightLimit) {
    StorageInventory small(branch,1500);
    small.addZone(zone);
    small.storeShipment(parcel,size);
    CHECK_EQUAL(0LL, small.remainingCapacity());
    CHECK_THROW(small.storeShipment(letter,size), std::invalid_argument);
    CHECK_EQUAL(1500LL, small.weightGrams());
    CHECK_EQUAL(std::size_t(1), small.shipmentCount());
}

TEST_FIXTURE(StorageFixture, StorageInventorySkipsLockedZonesAndRejectsUnavailableSpace) {
    StorageZone locked("LOCKED",13);
    locked.addCell(cell);
    locked.lock();
    inventory.addZone(locked);
    CHECK_THROW(inventory.storeShipment(letter,size), std::out_of_range);
    inventory.addZone(zone);
    CHECK_EQUAL(std::string("A"), inventory.storeShipment(letter,size).zoneId());
    CHECK_THROW(inventory.storeShipment(parcel,Dimensions(201,201,201)), std::out_of_range);
    parcel.dispatch();
    parcel.deliver();
    CHECK_THROW(inventory.storeShipment(parcel,size), std::logic_error);
}

TEST_FIXTURE(StorageFixture, SortingCenterValidatesIdentityAndShifts) {
    CHECK_THROW(SortingCenter(0,branch,3000), std::invalid_argument);
    CHECK_EQUAL(1, center.id());
    CHECK_EQUAL(13, center.branchId());
    CHECK(!center.isOpen());
    CHECK_THROW(center.closeShift(), std::logic_error);
    CHECK_THROW(center.receiveShipment(letter,size), std::logic_error);
    CHECK_THROW(center.createBag("B1",14,2000), std::logic_error);
    center.addZone(zone);
    center.openShift();
    CHECK(center.isOpen());
    CHECK_THROW(center.openShift(), std::logic_error);
    CHECK_THROW(center.addZone(StorageZone("OTHER",13)), std::logic_error);
    center.closeShift();
    CHECK(!center.isOpen());
}

TEST_FIXTURE(StorageFixture, SortingCenterMovesShipmentFromCellToOutboundBag) {
    center.addZone(zone);
    center.openShift();
    StorageLocation location = center.receiveShipment(letter,size);
    CHECK(center.locateShipment("LT001") == location);
    CHECK_EQUAL(std::size_t(1), center.storedCount());
    CHECK_EQUAL(20LL, center.storedWeightGrams());
    center.createBag("B1",14,2000);
    CHECK_EQUAL(std::size_t(1), center.bagCount());
    center.sortIntoBag(letter,"B1");
    CHECK_EQUAL(std::size_t(0), center.storedCount());
    CHECK_EQUAL(0LL, center.storedWeightGrams());
    CHECK(center.findBag("B1").contains("LT001"));
    CHECK_THROW(center.locateShipment("LT001"), std::out_of_range);
    CHECK_THROW(center.takeBag("B1"), std::logic_error);
    center.sealBag("B1");
    MailBag bag = center.takeBag("B1");
    CHECK(bag.sealed());
    CHECK_EQUAL(std::size_t(0), center.bagCount());
}

TEST_FIXTURE(StorageFixture, SortingFailureLeavesShipmentInStorage) {
    center.addZone(zone);
    center.openShift();
    center.receiveShipment(parcel,size);
    center.createBag("SMALL",14,20);
    CHECK_THROW(center.sortIntoBag(parcel,"SMALL"), CapacityExceededException);
    CHECK_EQUAL(std::size_t(1), center.storedCount());
    CHECK_EQUAL(std::size_t(0), center.findBag("SMALL").shipmentCount());
    CHECK_THROW(center.sortIntoBag(parcel,"missing"), std::out_of_range);
    CHECK_THROW(center.sortIntoBag(letter,"SMALL"), std::out_of_range);
    CHECK_EQUAL(1500LL, center.storedWeightGrams());
}

TEST_FIXTURE(StorageFixture, SortingCenterRejectsChangedWeightAndDeliveredShipment) {
    center.addZone(zone);
    center.openShift();
    center.receiveShipment(letter,size);
    center.createBag("B1",14,2000);
    Letter changed("LT001",sender,recipient,30,5);
    CHECK_THROW(center.sortIntoBag(changed,"B1"), std::invalid_argument);
    letter.dispatch();
    letter.deliver();
    CHECK_THROW(center.sortIntoBag(letter,"B1"), std::logic_error);
    CHECK_EQUAL(std::size_t(1), center.storedCount());
    CHECK_EQUAL(std::size_t(0), center.findBag("B1").shipmentCount());
}

TEST_FIXTURE(StorageFixture, SortingCenterRejectsRepeatedReceptionOfAlreadySortedShipment) {
    center.addZone(zone);
    center.openShift();
    center.receiveShipment(letter,size);
    center.createBag("B1",14,2000);
    center.sortIntoBag(letter,"B1");
    CHECK_THROW(center.receiveShipment(letter,size), std::invalid_argument);
    CHECK_EQUAL(std::size_t(0), center.storedCount());
    CHECK_EQUAL(std::size_t(1), center.findBag("B1").shipmentCount());
}

TEST_FIXTURE(StorageFixture, SortingCenterReportsDuplicateMissingAndClosedOperations) {
    center.openShift();
    center.createBag("B1",14,2000);
    CHECK_THROW(center.createBag("B1",14,2000), std::invalid_argument);
    CHECK_THROW(center.findBag("missing"), std::out_of_range);
    CHECK_THROW(center.sealBag("missing"), std::out_of_range);
    CHECK_THROW(center.takeBag("missing"), std::out_of_range);
    center.closeShift();
    CHECK_THROW(center.sortIntoBag(letter,"B1"), std::logic_error);
    CHECK_THROW(center.sealBag("B1"), std::logic_error);
    CHECK_THROW(center.takeBag("B1"), std::logic_error);
}

TEST_FIXTURE(StorageFixture, SortedBagCanBeTransportedToDestination) {
    center.addZone(zone);
    center.openShift();
    center.receiveShipment(parcel,size);
    center.createBag("B1",14,2000);
    center.sortIntoBag(parcel,"B1");
    center.sealBag("B1");
    MailBag bag = center.takeBag("B1");
    Route route(1,"Postal route");
    route.addStop(RouteStop(branch,0,0));
    route.addStop(RouteStop(otherBranch,100,120));
    Vehicle vehicle("1234 AB-7",2000,route);
    vehicle.loadBag(bag);
    vehicle.depart();
    vehicle.arriveAt(14);
    MailBag delivered = vehicle.unloadBag("B1");
    CHECK(delivered.contains("PK001"));
    CHECK_EQUAL(1500, delivered.weightGrams());
    CHECK_EQUAL(std::size_t(0), vehicle.bagCount());
}
