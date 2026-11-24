#include <UnitTest++/UnitTest++.h>
#include "registeredletter.h"
#include "expressletter.h"
#include "fragileparcel.h"
#include "insuredparcel.h"
#include "postalbranch.h"
#include "postalexception.h"
#include <limits>
#include <stdexcept>
#include <string>

struct SpecialShipmentFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    RegisteredLetter registered{"REG001", sender, recipient, 20, 5, 50};
    ExpressLetter express{"EXP001", sender, recipient, 20, 5, 24, 200};
    FragileParcel fragile{"FR001", sender, recipient, 1500, 200, 100, 50, true};
    InsuredParcel insured{"IN001", sender, recipient, 1500, 200, 100, 10000, 100};
};

TEST_FIXTURE(SpecialShipmentFixture, RegisteredLetterAddsRegistrationFee) {
    CHECK_EQUAL(50, registered.registrationFee());
    CHECK(registered.signedBy().empty());
    const Shipment &base = registered;
    CHECK_EQUAL(150, base.calculatePrice());
}

TEST_FIXTURE(SpecialShipmentFixture, RegisteredLetterRequiresDeliveryBeforeSignature) {
    CHECK_THROW(registered.confirmReceipt("Алексей"), std::logic_error);
    registered.dispatch();
    CHECK_THROW(registered.confirmReceipt("Алексей"), std::logic_error);
    CHECK(registered.signedBy().empty());
    registered.deliver();
    CHECK_THROW(registered.confirmReceipt(" \t\n"), std::invalid_argument);
    CHECK_THROW(registered.confirmReceipt(""), std::invalid_argument);
    registered.confirmReceipt("Алексей");
    CHECK_EQUAL(std::string("Алексей"), registered.signedBy());
    CHECK_THROW(registered.confirmReceipt("Другой"), std::logic_error);
    CHECK_EQUAL(std::string("Алексей"), registered.signedBy());
}

TEST_FIXTURE(SpecialShipmentFixture, RegisteredLetterChecksFeeAndOverflow) {
    CHECK_THROW(RegisteredLetter("REG002", sender, recipient, 1, 1, -1), std::invalid_argument);
    int maximum = std::numeric_limits<int>::max();
    CHECK_THROW(RegisteredLetter("REG002", sender, recipient, 1, 1, maximum), std::overflow_error);
    RegisteredLetter limit("REG002", sender, recipient, 1, 1, maximum - 1);
    CHECK_EQUAL(maximum, limit.calculatePrice());
}

TEST_FIXTURE(SpecialShipmentFixture, ExpressLetterDetectsOnlyPendingLateDelivery) {
    CHECK_EQUAL(24, express.deliveryHours());
    CHECK_EQUAL(200, express.expressFee());
    CHECK_EQUAL(300, express.calculatePrice());
    CHECK_THROW(express.isOverdue(-1), std::invalid_argument);
    CHECK(!express.isOverdue(25));
    express.dispatch();
    CHECK(!express.isOverdue(0));
    CHECK(!express.isOverdue(24));
    CHECK(express.isOverdue(25));
    express.deliver();
    CHECK(!express.isOverdue(25));
}

TEST_FIXTURE(SpecialShipmentFixture, ExpressLetterChecksDeadlineFeeAndOverflow) {
    CHECK_THROW(ExpressLetter("EXP002", sender, recipient, 1, 1, 0, 0), std::invalid_argument);
    CHECK_THROW(ExpressLetter("EXP002", sender, recipient, 1, 1, -1, 0), std::invalid_argument);
    CHECK_THROW(ExpressLetter("EXP002", sender, recipient, 1, 1, 24, -1), std::invalid_argument);
    int maximum = std::numeric_limits<int>::max();
    CHECK_THROW(ExpressLetter("EXP002", sender, recipient, 1, 1, 24, maximum), std::overflow_error);
    ExpressLetter limit("EXP002", sender, recipient, 1, 1, 24, maximum - 1);
    CHECK_EQUAL(maximum, limit.calculatePrice());
}

TEST_FIXTURE(SpecialShipmentFixture, FragileParcelChecksOrientationAndLabel) {
    CHECK_EQUAL(50, fragile.packagingFee());
    CHECK(fragile.keepUpright());
    CHECK_EQUAL(std::string("FRAGILE; KEEP UPRIGHT"), fragile.handlingLabel());
    fragile.checkOrientation(true);
    CHECK_THROW(fragile.checkOrientation(false), std::logic_error);
    CHECK_EQUAL(450, fragile.calculatePrice());
    FragileParcel freeOrientation("FR002", sender, recipient, 1, 0, 1, 0, false);
    freeOrientation.checkOrientation(false);
    CHECK(!freeOrientation.keepUpright());
    CHECK_EQUAL(std::string("FRAGILE"), freeOrientation.handlingLabel());
}

TEST_FIXTURE(SpecialShipmentFixture, FragileParcelChecksFeeAndOverflow) {
    CHECK_THROW(FragileParcel("FR002", sender, recipient, 1, 0, 1, -1, true), std::invalid_argument);
    int maximum = std::numeric_limits<int>::max();
    CHECK_THROW(FragileParcel("FR002", sender, recipient, 1, 0, 1, maximum, true), std::overflow_error);
    FragileParcel limit("FR002", sender, recipient, 1, 0, 1, maximum - 1, true);
    CHECK_EQUAL(maximum, limit.calculatePrice());
}

TEST_FIXTURE(SpecialShipmentFixture, InsuredParcelStoresCoverageAndAddsFee) {
    CHECK_EQUAL(10000, insured.declaredValue());
    CHECK_EQUAL(100, insured.insuranceFee());
    CHECK_EQUAL(10000, insured.remainingCoverage());
    CHECK_EQUAL(500, insured.calculatePrice());
}

TEST_FIXTURE(SpecialShipmentFixture, InsuredParcelLimitsRecordedCompensation) {
    CHECK_THROW(insured.recordCompensation(100), std::logic_error);
    insured.dispatch();
    CHECK_THROW(insured.recordCompensation(100), std::logic_error);
    insured.deliver();
    CHECK_THROW(insured.recordCompensation(0), std::invalid_argument);
    CHECK_THROW(insured.recordCompensation(-1), std::invalid_argument);
    CHECK_THROW(insured.recordCompensation(10001), std::invalid_argument);
    insured.recordCompensation(2500);
    CHECK_EQUAL(7500, insured.remainingCoverage());
    CHECK_THROW(insured.recordCompensation(7501), std::invalid_argument);
    insured.recordCompensation(7500);
    CHECK_EQUAL(0, insured.remainingCoverage());
    CHECK_THROW(insured.recordCompensation(1), std::invalid_argument);
}

TEST_FIXTURE(SpecialShipmentFixture, InsuredParcelChecksValueFeeAndOverflow) {
    CHECK_THROW(InsuredParcel("IN002", sender, recipient, 1, 0, 1, 0, 0), std::invalid_argument);
    CHECK_THROW(InsuredParcel("IN002", sender, recipient, 1, 0, 1, -1, 0), std::invalid_argument);
    CHECK_THROW(InsuredParcel("IN002", sender, recipient, 1, 0, 1, 100, -1), std::invalid_argument);
    int maximum = std::numeric_limits<int>::max();
    CHECK_THROW(InsuredParcel("IN002", sender, recipient, 1, 0, 1, 100, maximum), std::overflow_error);
    InsuredParcel limit("IN002", sender, recipient, 1, 0, 1, 100, maximum - 1);
    CHECK_EQUAL(maximum, limit.calculatePrice());
}

TEST_FIXTURE(SpecialShipmentFixture, EverySpecialShipmentClonesItsFullType) {
    auto regCopy = registered.clone();
    auto expCopy = express.clone();
    auto frCopy = fragile.clone();
    auto inCopy = insured.clone();
    CHECK(dynamic_cast<const RegisteredLetter *>(regCopy.get()) != nullptr);
    CHECK(dynamic_cast<const ExpressLetter *>(expCopy.get()) != nullptr);
    CHECK(dynamic_cast<const FragileParcel *>(frCopy.get()) != nullptr);
    CHECK(dynamic_cast<const InsuredParcel *>(inCopy.get()) != nullptr);
    CHECK_EQUAL(150, regCopy->calculatePrice());
    CHECK_EQUAL(300, expCopy->calculatePrice());
    CHECK_EQUAL(450, frCopy->calculatePrice());
    CHECK_EQUAL(500, inCopy->calculatePrice());
    regCopy->dispatch();
    CHECK(registered.status() == ShipmentStatus::Accepted);
}

TEST_FIXTURE(SpecialShipmentFixture, ClonesPreserveReceiptAndCompensationState) {
    registered.dispatch();
    registered.deliver();
    registered.confirmReceipt("Алексей");
    auto regCopy = registered.clone();
    const RegisteredLetter *reg = dynamic_cast<const RegisteredLetter *>(regCopy.get());
    CHECK(reg != nullptr);
    if(reg){
        CHECK_EQUAL(std::string("Алексей"), reg->signedBy());
    }
    insured.dispatch();
    insured.deliver();
    insured.recordCompensation(1000);
    auto inCopy = insured.clone();
    const InsuredParcel *ins = dynamic_cast<const InsuredParcel *>(inCopy.get());
    CHECK(ins != nullptr);
    if(ins){
        CHECK_EQUAL(9000, ins->remainingCoverage());
    }
}

TEST_FIXTURE(SpecialShipmentFixture, BranchHandlesAllSpecialShipmentsPolymorphically) {
    PostalBranch branch(13, address);
    const Shipment *shipments[] = {&registered, &express, &fragile, &insured};
    const int prices[] = {150, 300, 450, 500};
    for(int i = 0; i < 4; i++){
        branch.acceptShipment(*shipments[i]);
        const std::string &number = shipments[i]->trackingNumber();
        CHECK_EQUAL(prices[i], branch.findShipment(number).calculatePrice());
        branch.dispatchShipment(number);
        branch.deliverShipment(number);
        CHECK(branch.findShipment(number).status() == ShipmentStatus::Delivered);
        CHECK(shipments[i]->status() == ShipmentStatus::Accepted);
    }
    CHECK_EQUAL(std::size_t(4), branch.shipmentCount());
}

TEST_FIXTURE(SpecialShipmentFixture, BranchConfirmsStoredRegisteredLetterReceipt) {
    PostalBranch branch(13, address);
    branch.acceptShipment(registered);
    branch.acceptShipment(express);
    CHECK_THROW(branch.confirmReceipt("REG001", "Алексей"), std::logic_error);
    CHECK_THROW(branch.confirmReceipt("EXP001", "Алексей"), std::invalid_argument);
    branch.dispatchShipment("REG001");
    branch.deliverShipment("REG001");
    CHECK_THROW(branch.confirmReceipt("REG001", ""), std::invalid_argument);
    branch.confirmReceipt("REG001", "Алексей");
    const RegisteredLetter *stored = dynamic_cast<const RegisteredLetter *>(&branch.findShipment("REG001"));
    CHECK(stored != nullptr);
    if(stored){
        CHECK_EQUAL(std::string("Алексей"), stored->signedBy());
    }
    CHECK(registered.signedBy().empty());
    CHECK_THROW(branch.confirmReceipt("REG001", "Алексей"), std::logic_error);
    CHECK_THROW(branch.confirmReceipt("missing", "Алексей"), PostalException);
}

TEST_FIXTURE(SpecialShipmentFixture, BranchRecordsCompensationForStoredInsuredParcel) {
    PostalBranch branch(13, address);
    branch.acceptShipment(insured);
    branch.acceptShipment(fragile);
    CHECK_THROW(branch.recordCompensation("IN001", 1000), std::logic_error);
    CHECK_THROW(branch.recordCompensation("FR001", 1000), std::invalid_argument);
    branch.dispatchShipment("IN001");
    branch.deliverShipment("IN001");
    branch.recordCompensation("IN001", 1000);
    const InsuredParcel *stored = dynamic_cast<const InsuredParcel *>(&branch.findShipment("IN001"));
    CHECK(stored != nullptr);
    if(stored){
        CHECK_EQUAL(9000, stored->remainingCoverage());
    }
    CHECK_EQUAL(10000, insured.remainingCoverage());
    CHECK_THROW(branch.recordCompensation("IN001", 9001), std::invalid_argument);
    CHECK_THROW(branch.recordCompensation("missing", 1000), PostalException);
}
