#include "branchnotfoundexception.h"
#include "customernotfoundexception.h"
#include "duplicateshipmentexception.h"
#include "shipmentstateexception.h"
#include <UnitTest++/UnitTest++.h>
#include "postalnetwork.h"
#include "letter.h"
#include "parcel.h"
#include "registeredletter.h"
#include "postalexception.h"
#include <stdexcept>
#include <string>

struct NetworkFixture {
    PostalAddress address{"Минск","Гикало","9","220013"};
    PostalAddress otherAddress{"Брест","Советская","1","224000"};
    Customer sender{1,"Владислав",address};
    Customer recipient{2,"Алексей",address};
    Letter letter{"LT001",sender,recipient,20,5};
    Parcel parcel{"PK001",sender,recipient,1500,200,100};
    PostalNetwork network{"Учебная почта"};
    void configureNetwork() {
        network.registerCustomer(sender);
        network.registerCustomer(recipient);
        network.openBranch(13,address);
        network.openBranch(14,otherAddress);
    }
};

TEST_FIXTURE(NetworkFixture, CustomerDirectoryRegistersAndFindsUniqueCustomers) {
    CustomerDirectory directory;
    CHECK_EQUAL(std::size_t(0),directory.size());
    CHECK(!directory.contains(1));
    directory.add(sender);
    const Customer &stored=directory.find(1);
    directory.add(recipient);
    CHECK_EQUAL(std::size_t(2),directory.size());
    CHECK_EQUAL(1,stored.id());
    CHECK_EQUAL(std::string("Владислав"),stored.name());
    CHECK(directory.contains(2));
    CHECK_THROW(directory.add(sender),std::invalid_argument);
    CHECK_THROW(directory.find(99),CustomerNotFoundException);
}

TEST_FIXTURE(NetworkFixture, CustomerDirectoryChangesAddressAndRemovesRecords) {
    CustomerDirectory directory;
    directory.add(sender);
    directory.changeAddress(1,otherAddress);
    CHECK_EQUAL(otherAddress.formatLabel(),directory.find(1).address().formatLabel());
    CHECK_EQUAL(address.formatLabel(),sender.address().formatLabel());
    CHECK_THROW(directory.changeAddress(99,otherAddress),CustomerNotFoundException);
    directory.remove(1);
    CHECK(!directory.contains(1));
    CHECK_EQUAL(std::size_t(0),directory.size());
    CHECK_THROW(directory.remove(1),CustomerNotFoundException);
}

TEST_FIXTURE(NetworkFixture, BranchDirectoryCreatesUniqueBranchesAndKeepsStableReferences) {
    BranchDirectory directory;
    CHECK_EQUAL(std::size_t(0),directory.size());
    CHECK(!directory.contains(13));
    CHECK_THROW(directory.add(0,address),std::invalid_argument);
    directory.add(13,address);
    const PostalBranch &stored=directory.find(13);
    directory.add(14,otherAddress);
    CHECK_EQUAL(13,stored.id());
    CHECK_EQUAL(address.formatLabel(),stored.address().formatLabel());
    CHECK_EQUAL(std::size_t(2),directory.size());
    CHECK(directory.contains(14));
    CHECK_THROW(directory.add(13,otherAddress),std::invalid_argument);
    CHECK_THROW(directory.find(99),BranchNotFoundException);
}

TEST_FIXTURE(NetworkFixture, BranchDirectoryRemovesOnlyEmptyBranches) {
    BranchDirectory directory;
    directory.add(13,address);
    directory.add(14,otherAddress);
    directory.acceptShipment(13,letter);
    CHECK_THROW(directory.remove(13),std::logic_error);
    directory.remove(14);
    CHECK(!directory.contains(14));
    CHECK_EQUAL(std::size_t(1),directory.size());
    CHECK_THROW(directory.remove(99),BranchNotFoundException);
}

TEST_FIXTURE(NetworkFixture, BranchDirectoryLocatesShipmentsAndRejectsGlobalDuplicateTracking) {
    BranchDirectory directory;
    directory.add(13,address);
    directory.add(14,otherAddress);
    CHECK_EQUAL(std::size_t(0),directory.shipmentCount());
    CHECK(!directory.containsShipment("LT001"));
    directory.acceptShipment(13,letter);
    CHECK(directory.containsShipment("LT001"));
    CHECK_EQUAL(13,directory.locateShipment("LT001"));
    CHECK_THROW(directory.acceptShipment(14,letter),DuplicateShipmentException);
    CHECK_EQUAL(std::size_t(0),directory.find(14).shipmentCount());
    CHECK_EQUAL(std::size_t(1),directory.shipmentCount());
    CHECK_THROW(directory.locateShipment("missing"),std::out_of_range);
    CHECK_THROW(directory.acceptShipment(99,parcel),BranchNotFoundException);
}

TEST_FIXTURE(NetworkFixture, BranchDirectoryDispatchesAndDeliversStoredShipment) {
    BranchDirectory directory;
    directory.add(13,address);
    directory.acceptShipment(13,letter);
    CHECK_THROW(directory.deliverShipment(13,"LT001"),ShipmentStateException);
    directory.dispatchShipment(13,"LT001");
    CHECK(directory.find(13).findShipment("LT001").status()==ShipmentStatus::InTransit);
    directory.deliverShipment(13,"LT001");
    CHECK(directory.find(13).findShipment("LT001").status()==ShipmentStatus::Delivered);
    CHECK_THROW(directory.dispatchShipment(99,"LT001"),BranchNotFoundException);
    CHECK_THROW(directory.deliverShipment(99,"LT001"),BranchNotFoundException);
    CHECK_THROW(directory.dispatchShipment(13,"missing"),PostalException);
}

TEST_FIXTURE(NetworkFixture, PostalBranchReportsPresenceWithoutThrowing) {
    PostalBranch branch(13,address);
    CHECK(!branch.containsShipment("LT001"));
    branch.acceptShipment(letter);
    CHECK(branch.containsShipment("LT001"));
    CHECK(!branch.containsShipment("missing"));
}

TEST(PostalNetworkValidatesName) {
    CHECK_THROW(PostalNetwork(""),std::invalid_argument);
    CHECK_THROW(PostalNetwork(" \t\n"),std::invalid_argument);
    PostalNetwork network("Учебная почта");
    CHECK_EQUAL(std::string("Учебная почта"),network.name());
    CHECK_EQUAL(std::size_t(0),network.customerCount());
    CHECK_EQUAL(std::size_t(0),network.branchCount());
    CHECK_EQUAL(std::size_t(0),network.shipmentCount());
}

TEST_FIXTURE(NetworkFixture, PostalNetworkRegistersCustomersAndBranches) {
    configureNetwork();
    CHECK_EQUAL(std::size_t(2),network.customerCount());
    CHECK_EQUAL(std::size_t(2),network.branchCount());
    CHECK_EQUAL(1,network.customer(1).id());
    CHECK_EQUAL(13,network.branch(13).id());
    CHECK_THROW(network.registerCustomer(sender),std::invalid_argument);
    CHECK_THROW(network.openBranch(13,address),std::invalid_argument);
    CHECK_THROW(network.customer(99),CustomerNotFoundException);
    CHECK_THROW(network.branch(99),BranchNotFoundException);
}

TEST_FIXTURE(NetworkFixture, PostalNetworkRequiresBothParticipantsToBeRegistered) {
    network.openBranch(13,address);
    CHECK_THROW(network.acceptShipment(13,letter),std::invalid_argument);
    network.registerCustomer(sender);
    CHECK_THROW(network.acceptShipment(13,letter),std::invalid_argument);
    network.registerCustomer(recipient);
    network.acceptShipment(13,letter);
    CHECK_EQUAL(std::size_t(1),network.shipmentCount());
}

TEST_FIXTURE(NetworkFixture, PostalNetworkPreservesPolymorphicTypesAndSearchesAcrossBranches) {
    configureNetwork();
    RegisteredLetter registered("RG001",sender,recipient,20,5,50);
    network.acceptShipment(13,registered);
    network.acceptShipment(14,parcel);
    CHECK_EQUAL(13,network.shipmentBranchId("RG001"));
    CHECK_EQUAL(14,network.shipmentBranchId("PK001"));
    CHECK_EQUAL(150,network.findShipment("RG001").calculatePrice());
    CHECK_EQUAL(400,network.findShipment("PK001").calculatePrice());
    CHECK(dynamic_cast<const RegisteredLetter *>(&network.findShipment("RG001"))!=nullptr);
    CHECK(dynamic_cast<const Parcel *>(&network.findShipment("PK001"))!=nullptr);
    CHECK_EQUAL(std::size_t(2),network.shipmentCount());
}

TEST_FIXTURE(NetworkFixture, PostalNetworkChangesOnlyStoredShipmentStatuses) {
    configureNetwork();
    network.acceptShipment(13,letter);
    CHECK_THROW(network.deliverShipment("LT001"),ShipmentStateException);
    network.dispatchShipment("LT001");
    CHECK(network.findShipment("LT001").status()==ShipmentStatus::InTransit);
    network.deliverShipment("LT001");
    CHECK(network.findShipment("LT001").status()==ShipmentStatus::Delivered);
    CHECK(letter.status()==ShipmentStatus::Accepted);
    CHECK_THROW(network.dispatchShipment("LT001"),ShipmentStateException);
}

TEST_FIXTURE(NetworkFixture, PostalNetworkKeepsShipmentAddressSnapshotAfterCustomerMoves) {
    configureNetwork();
    Letter stored("SNAP001",network.customer(1),network.customer(2),20,5);
    network.acceptShipment(13,stored);
    network.changeCustomerAddress(2,otherAddress);
    CHECK_EQUAL(otherAddress.formatLabel(),network.customer(2).address().formatLabel());
    CHECK_EQUAL(address.formatLabel(),network.findShipment("SNAP001").recipient().address().formatLabel());
    CHECK_THROW(network.changeCustomerAddress(99,otherAddress),CustomerNotFoundException);
}

TEST_FIXTURE(NetworkFixture, PostalNetworkRejectsDuplicateAndUnknownShipmentOperations) {
    configureNetwork();
    network.acceptShipment(13,letter);
    CHECK_THROW(network.acceptShipment(14,letter),DuplicateShipmentException);
    CHECK_EQUAL(std::size_t(0),network.branch(14).shipmentCount());
    CHECK_THROW(network.findShipment("missing"),std::out_of_range);
    CHECK_THROW(network.shipmentBranchId("missing"),std::out_of_range);
    CHECK_THROW(network.dispatchShipment("missing"),std::out_of_range);
    CHECK_THROW(network.deliverShipment("missing"),std::out_of_range);
}
