#include <UnitTest++/UnitTest++.h>
#include "requestregistry.h"
#include "returnrequest.h"
#include "redirectionrequest.h"
#include "damageclaim.h"
#include "insuredparcel.h"
#include "letter.h"
#include "postalexception.h"
#include <stdexcept>
#include <string>
#include <type_traits>

static_assert(std::is_abstract<CustomerRequest>::value, "Requests must have a concrete type");
static_assert(std::has_virtual_destructor<CustomerRequest>::value, "Request destructor must be virtual");
static_assert(!std::is_assignable<CustomerRequest &,const CustomerRequest &>::value, "Base assignment must stay disabled");

struct RequestFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    PostalAddress newAddress{"Брест", "Советская", "1", "224000"};
    PostalBranch branch{13, address};
    PostalBranch otherBranch{14, address};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Customer stranger{3, "Другой", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    InsuredParcel insured{"IN001", sender, recipient, 1000, 100, 100, 10000, 100};
    RequestRegistry registry{branch};
    RequestFixture() {
        branch.acceptShipment(letter);
        branch.acceptShipment(insured);
    }
    void deliverInsured() {
        branch.dispatchShipment("IN001");
        branch.deliverShipment("IN001");
    }
};

TEST_FIXTURE(RequestFixture, CustomerRequestStoresApplicantAndInitialState) {
    ReturnRequest request(1,sender,letter,10,"Recall",50);
    CHECK_EQUAL(1, request.id());
    CHECK_EQUAL(1, request.applicant().id());
    CHECK_EQUAL(std::string("LT001"), request.trackingNumber());
    CHECK_EQUAL(10, request.openedMinute());
    CHECK_EQUAL(std::string("Recall"), request.reason());
    CHECK(request.status() == RequestStatus::Open);
    CHECK(request.decisionReason().empty());
    CHECK_EQUAL(50, request.returnFee());
    CHECK_EQUAL(address.formatLabel(), request.returnAddress().formatLabel());
}

TEST_FIXTURE(RequestFixture, CustomerRequestValidatesIdentityReasonAndParticipation) {
    CHECK_THROW(ReturnRequest(0,sender,letter,0,"reason",0), std::invalid_argument);
    CHECK_THROW(ReturnRequest(1,sender,letter,-1,"reason",0), std::invalid_argument);
    CHECK_THROW(ReturnRequest(1,sender,letter,0,"",0), std::invalid_argument);
    CHECK_THROW(ReturnRequest(1,sender,letter,0," \t\n",0), std::invalid_argument);
    CHECK_THROW(ReturnRequest(1,stranger,letter,0,"reason",0), std::invalid_argument);
    CHECK_THROW(ReturnRequest(1,sender,letter,0,"reason",-1), std::invalid_argument);
}

TEST_FIXTURE(RequestFixture, ReturnRequestApprovesOnceAndIssuesSenderLabel) {
    ReturnRequest request(1,sender,letter,0,"Recall",50);
    CHECK_THROW(request.issueLabel(letter), std::logic_error);
    request.approve(letter);
    CHECK(request.status() == RequestStatus::Approved);
    CHECK_THROW(request.approve(letter), std::logic_error);
    std::string label = request.issueLabel(letter);
    CHECK(label.find("RETURN LT001 TO " + address.formatLabel()) != std::string::npos);
    CHECK(label.find("50 kopecks") != std::string::npos);
    CHECK(request.status() == RequestStatus::Completed);
    CHECK_THROW(request.issueLabel(letter), std::logic_error);
}

TEST_FIXTURE(RequestFixture, ReturnRequestRejectsRecipientRecallDeliveredShipmentAndWrongTracking) {
    ReturnRequest recipientRequest(1,recipient,letter,0,"Recall",0);
    CHECK_THROW(recipientRequest.approve(letter), std::invalid_argument);
    ReturnRequest request(2,sender,letter,0,"Recall",0);
    CHECK_THROW(request.approve(insured), std::invalid_argument);
    letter.dispatch();
    letter.deliver();
    CHECK_THROW(request.approve(letter), std::logic_error);
    CHECK(request.status() == RequestStatus::Open);
}

TEST_FIXTURE(RequestFixture, ReturnLabelRechecksDeliveryAfterApproval) {
    ReturnRequest request(1,sender,letter,0,"Recall",0);
    request.approve(letter);
    letter.dispatch();
    letter.deliver();
    CHECK_THROW(request.issueLabel(letter), std::logic_error);
    CHECK(request.status() == RequestStatus::Approved);
}

TEST_FIXTURE(RequestFixture, RejectionStoresReasonAndIsTerminal) {
    ReturnRequest request(1,sender,letter,0,"Recall",0);
    CHECK_THROW(request.reject(" \t"), std::invalid_argument);
    request.reject("Not eligible");
    CHECK(request.status() == RequestStatus::Rejected);
    CHECK_EQUAL(std::string("Not eligible"), request.decisionReason());
    CHECK_THROW(request.reject("Again"), std::logic_error);
    CHECK_THROW(request.approve(letter), std::logic_error);
    ReturnRequest approved(2,sender,letter,0,"Recall",0);
    approved.approve(letter);
    CHECK_THROW(approved.reject("Too late"), std::logic_error);
}

TEST_FIXTURE(RequestFixture, RedirectionRequestIssuesNewLabelWithoutChangingOriginalAddress) {
    RedirectionRequest request(1,recipient,letter,0,"Moved",newAddress,70);
    CHECK_EQUAL(70, request.redirectionFee());
    CHECK_EQUAL(newAddress.formatLabel(), request.destination().formatLabel());
    CHECK_THROW(request.issueLabel(letter), std::logic_error);
    request.approve(letter);
    std::string label = request.issueLabel(letter);
    CHECK(label.find("REDIRECT LT001 TO " + newAddress.formatLabel()) != std::string::npos);
    CHECK(request.status() == RequestStatus::Completed);
    CHECK_EQUAL(address.formatLabel(), letter.recipient().address().formatLabel());
}

TEST_FIXTURE(RequestFixture, RedirectionRequestRejectsSameAddressDeliveredShipmentAndNegativeFee) {
    CHECK_THROW(RedirectionRequest(1,sender,letter,0,"Moved",newAddress,-1), std::invalid_argument);
    RedirectionRequest unchanged(1,sender,letter,0,"Moved",address,0);
    CHECK_THROW(unchanged.approve(letter), std::invalid_argument);
    RedirectionRequest request(2,sender,letter,0,"Moved",newAddress,0);
    request.approve(letter);
    letter.dispatch();
    letter.deliver();
    CHECK_THROW(request.issueLabel(letter), std::logic_error);
    RedirectionRequest late(3,sender,letter,0,"Moved",newAddress,0);
    CHECK_THROW(late.approve(letter), std::logic_error);
}

TEST_FIXTURE(RequestFixture, DamageClaimValidatesInsuranceDeliveryAndAmount) {
    CHECK_THROW(DamageClaim(1,recipient,insured,0,"Damage",0), std::invalid_argument);
    DamageClaim claim(1,recipient,insured,0,"Damage",1000);
    CHECK_EQUAL(1000, claim.requestedAmount());
    CHECK_EQUAL(0, claim.settledAmount());
    CHECK_THROW(claim.approve(insured), std::logic_error);
    DamageClaim wrongType(2,recipient,letter,0,"Damage",100);
    CHECK_THROW(wrongType.approve(letter), std::invalid_argument);
    deliverInsured();
    DamageClaim excess(3,recipient,insured,0,"Damage",10001);
    CHECK_THROW(excess.approve(branch.findShipment("IN001")), std::invalid_argument);
}

TEST_FIXTURE(RequestFixture, DamageClaimSettlesAgainstStoredInsuredParcelOnce) {
    DamageClaim claim(1,recipient,insured,0,"Damage",2500);
    CHECK_THROW(claim.settle(branch), std::logic_error);
    deliverInsured();
    claim.approve(branch.findShipment("IN001"));
    claim.settle(branch);
    CHECK_EQUAL(2500, claim.settledAmount());
    CHECK(claim.status() == RequestStatus::Completed);
    const auto *parcel = dynamic_cast<const InsuredParcel *>(&branch.findShipment("IN001"));
    CHECK(parcel != nullptr);
    if(parcel){ CHECK_EQUAL(7500, parcel->remainingCoverage()); }
    CHECK_THROW(claim.settle(branch), std::logic_error);
    CHECK_EQUAL(10000, insured.remainingCoverage());
}

TEST_FIXTURE(RequestFixture, DamageClaimRechecksCoverageBeforeSettlement) {
    deliverInsured();
    DamageClaim claim(1,recipient,insured,0,"Damage",8000);
    claim.approve(branch.findShipment("IN001"));
    branch.recordCompensation("IN001",3000);
    CHECK_THROW(claim.settle(branch), std::invalid_argument);
    CHECK(claim.status() == RequestStatus::Approved);
    CHECK_EQUAL(0, claim.settledAmount());
}

TEST_FIXTURE(RequestFixture, RequestClonesPreserveConcreteTypeAndIndependentState) {
    ReturnRequest recall(1,sender,letter,0,"Recall",50);
    RedirectionRequest redirection(2,sender,letter,0,"Moved",newAddress,70);
    DamageClaim claim(3,recipient,insured,0,"Damage",1000);
    auto first = recall.clone();
    auto second = redirection.clone();
    auto third = claim.clone();
    CHECK(dynamic_cast<const ReturnRequest *>(first.get()) != nullptr);
    CHECK(dynamic_cast<const RedirectionRequest *>(second.get()) != nullptr);
    CHECK(dynamic_cast<const DamageClaim *>(third.get()) != nullptr);
    first->reject("Rejected copy");
    CHECK(recall.status() == RequestStatus::Open);
    CHECK(first->status() == RequestStatus::Rejected);
}

TEST_FIXTURE(RequestFixture, RegistryStoresOnlyOpenUniqueRequestsAtCorrectBranch) {
    ReturnRequest request(1,sender,letter,0,"Recall",50);
    CHECK_EQUAL(13, registry.branchId());
    CHECK_EQUAL(std::size_t(0), registry.requestCount());
    CHECK_THROW(registry.submit(otherBranch,request), std::invalid_argument);
    registry.submit(branch,request);
    CHECK_EQUAL(std::size_t(1), registry.requestCount());
    CHECK_THROW(registry.submit(branch,request), std::invalid_argument);
    request.reject("Rejected outside");
    CHECK(registry.findRequest(1).status() == RequestStatus::Open);
    CHECK_THROW(registry.submit(branch,request), std::logic_error);
}

TEST_FIXTURE(RequestFixture, RegistryApprovesAndIssuesReturnAndRedirectionLabels) {
    ReturnRequest recall(1,sender,letter,0,"Recall",50);
    RedirectionRequest redirection(2,recipient,letter,0,"Moved",newAddress,70);
    registry.submit(branch,recall);
    registry.submit(branch,redirection);
    registry.approve(1,branch);
    registry.approve(2,branch);
    CHECK(registry.issueReturnLabel(1,branch).find("RETURN LT001") != std::string::npos);
    CHECK(registry.issueRedirectionLabel(2,branch).find("REDIRECT LT001") != std::string::npos);
    CHECK(registry.findRequest(1).status() == RequestStatus::Completed);
    CHECK(registry.findRequest(2).status() == RequestStatus::Completed);
}

TEST_FIXTURE(RequestFixture, RegistryRejectsIneligibleCompletionsAndWrongRequestTypes) {
    ReturnRequest recall(1,sender,letter,0,"Recall",0);
    RedirectionRequest redirection(2,recipient,letter,0,"Moved",newAddress,0);
    DamageClaim claim(3,recipient,insured,0,"Damage",1000);
    registry.submit(branch,recall);
    registry.submit(branch,redirection);
    registry.submit(branch,claim);
    CHECK_THROW(registry.issueReturnLabel(2,branch), std::invalid_argument);
    CHECK_THROW(registry.issueRedirectionLabel(1,branch), std::invalid_argument);
    CHECK_THROW(registry.settleDamageClaim(1,branch), std::invalid_argument);
    CHECK_THROW(registry.issueReturnLabel(1,branch), std::logic_error);
    CHECK_THROW(registry.approve(1,otherBranch), std::invalid_argument);
    CHECK_THROW(registry.issueReturnLabel(1,otherBranch), std::invalid_argument);
    CHECK_THROW(registry.issueRedirectionLabel(2,otherBranch), std::invalid_argument);
    CHECK_THROW(registry.settleDamageClaim(3,otherBranch), std::invalid_argument);
}

TEST_FIXTURE(RequestFixture, RegistryRechecksDeliveredStateAndSettlesStoredClaim) {
    ReturnRequest recall(1,sender,letter,0,"Recall",0);
    RedirectionRequest redirection(2,sender,letter,0,"Moved",newAddress,0);
    DamageClaim claim(3,recipient,insured,0,"Damage",1000);
    registry.submit(branch,recall);
    registry.submit(branch,redirection);
    registry.submit(branch,claim);
    registry.approve(1,branch);
    registry.approve(2,branch);
    branch.dispatchShipment("LT001");
    branch.deliverShipment("LT001");
    CHECK_THROW(registry.issueReturnLabel(1,branch), std::logic_error);
    CHECK_THROW(registry.issueRedirectionLabel(2,branch), std::logic_error);
    deliverInsured();
    registry.approve(3,branch);
    registry.settleDamageClaim(3,branch);
    const auto *stored = dynamic_cast<const DamageClaim *>(&registry.findRequest(3));
    CHECK(stored != nullptr);
    if(stored){ CHECK_EQUAL(1000, stored->settledAmount()); }
    CHECK(claim.status() == RequestStatus::Open);
}

TEST_FIXTURE(RequestFixture, RegistryHandlesRejectionMissingRequestsAndUnknownShipment) {
    ReturnRequest recall(1,sender,letter,0,"Recall",0);
    registry.submit(branch,recall);
    registry.reject(1,"Not eligible");
    CHECK(registry.findRequest(1).status() == RequestStatus::Rejected);
    CHECK_THROW(registry.findRequest(99), std::out_of_range);
    CHECK_THROW(registry.reject(99,"reason"), std::out_of_range);
    CHECK_THROW(registry.approve(99,branch), std::out_of_range);
    Letter unknown("UNKNOWN",sender,recipient,20,5);
    ReturnRequest missing(2,sender,unknown,0,"Recall",0);
    CHECK_THROW(registry.submit(branch,missing), PostalException);
}

TEST_FIXTURE(RequestFixture, RegistryRejectsApplicantNotInCurrentShipmentRecord) {
    Customer differentSender(10,"Подмена",address);
    Customer differentRecipient(11,"Подмена",address);
    Letter mismatched("LT001",differentSender,differentRecipient,20,5);
    ReturnRequest request(1,differentSender,mismatched,0,"Recall",0);
    CHECK_THROW(registry.submit(branch,request), std::invalid_argument);
    CHECK_EQUAL(std::size_t(0), registry.requestCount());
}
