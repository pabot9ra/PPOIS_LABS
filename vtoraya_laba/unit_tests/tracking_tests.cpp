#include <UnitTest++/UnitTest++.h>
#include "trackinghistory.h"
#include "notificationservice.h"
#include "letter.h"
#include "postalexception.h"
#include <stdexcept>
#include <string>

struct TrackingFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    PostalBranch branch{13, address};
    PostalBranch otherBranch{14, address};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    TrackingHistory history{letter};
    NotificationService service{branch};
    TrackingFixture() {
        branch.acceptShipment(letter);
        otherBranch.acceptShipment(letter);
    }
    void deliver() {
        branch.dispatchShipment("LT001");
        branch.deliverShipment("LT001");
    }
};

TEST_FIXTURE(TrackingFixture, TrackingEventCapturesImmutableBranchState) {
    TrackingEvent event(branch, "LT001", 10, "Принято");
    branch.dispatchShipment("LT001");
    CHECK_EQUAL(std::string("LT001"), event.trackingNumber());
    CHECK(event.status() == ShipmentStatus::Accepted);
    CHECK_EQUAL(13, event.branchId());
    CHECK_EQUAL(10, event.minute());
    CHECK_EQUAL(std::string("Принято"), event.description());
    CHECK(event.format().find("LT001, branch 13, minute 10") != std::string::npos);
}

TEST_FIXTURE(TrackingFixture, TrackingEventValidatesTimeDescriptionAndShipmentExistence) {
    CHECK_THROW(TrackingEvent(branch, "LT001", -1, "event"), std::invalid_argument);
    CHECK_THROW(TrackingEvent(branch, "LT001", 0, ""), std::invalid_argument);
    CHECK_THROW(TrackingEvent(branch, "LT001", 0, " \t\n"), std::invalid_argument);
    CHECK_THROW(TrackingEvent(branch, "missing", 0, "event"), PostalException);
}

TEST_FIXTURE(TrackingFixture, EmptyTrackingHistoryHasNoEventsOrElapsedTime) {
    CHECK_EQUAL(std::string("LT001"), history.trackingNumber());
    CHECK_EQUAL(std::size_t(0), history.eventCount());
    CHECK_EQUAL(0, history.elapsedMinutes());
    CHECK(!history.hasStatus(ShipmentStatus::Accepted));
    CHECK_THROW(history.lastEvent(), std::out_of_range);
    CHECK_THROW(history.eventAt(0), std::out_of_range);
    CHECK_THROW(history.findLastAtBranch(13), std::out_of_range);
}

TEST_FIXTURE(TrackingFixture, TrackingHistoryRecordsFullDeliverySequence) {
    history.record(TrackingEvent(branch, "LT001", 10, "Accepted"));
    branch.dispatchShipment("LT001");
    history.record(TrackingEvent(branch, "LT001", 20, "Dispatched"));
    branch.deliverShipment("LT001");
    history.record(TrackingEvent(branch, "LT001", 40, "Delivered"));
    CHECK_EQUAL(std::size_t(3), history.eventCount());
    CHECK_EQUAL(30, history.elapsedMinutes());
    CHECK(history.eventAt(1).status() == ShipmentStatus::InTransit);
    CHECK(history.lastEvent().status() == ShipmentStatus::Delivered);
    CHECK(history.hasStatus(ShipmentStatus::Accepted));
    CHECK(history.hasStatus(ShipmentStatus::InTransit));
    CHECK(history.hasStatus(ShipmentStatus::Delivered));
    CHECK_EQUAL(40, history.findLastAtBranch(13).minute());
}

TEST_FIXTURE(TrackingFixture, TrackingHistoryRejectsAnotherShipmentAndPreservesEvents) {
    Letter other("LT002", sender, recipient, 10, 5);
    branch.acceptShipment(other);
    CHECK_THROW(history.record(TrackingEvent(branch, "LT002", 0, "Other")), std::invalid_argument);
    CHECK_EQUAL(std::size_t(0), history.eventCount());
}

TEST_FIXTURE(TrackingFixture, TrackingHistoryRequiresAcceptanceAndDispatchEvents) {
    branch.dispatchShipment("LT001");
    TrackingEvent transit(branch, "LT001", 10, "Dispatched");
    CHECK_THROW(history.record(transit), std::logic_error);
    history.record(TrackingEvent(otherBranch, "LT001", 0, "Accepted"));
    branch.deliverShipment("LT001");
    CHECK_THROW(history.record(TrackingEvent(branch, "LT001", 20, "Delivered")), std::logic_error);
    CHECK_EQUAL(std::size_t(1), history.eventCount());
    history.record(transit);
    history.record(TrackingEvent(branch, "LT001", 20, "Delivered"));
    CHECK_EQUAL(std::size_t(3), history.eventCount());
}

TEST_FIXTURE(TrackingFixture, TrackingHistoryRejectsTimeAndStatusRegression) {
    TrackingEvent accepted(branch, "LT001", 10, "Accepted");
    history.record(accepted);
    CHECK_THROW(history.record(TrackingEvent(branch, "LT001", 9, "Earlier")), std::invalid_argument);
    branch.dispatchShipment("LT001");
    history.record(TrackingEvent(branch, "LT001", 10, "Dispatched in same minute"));
    CHECK_THROW(history.record(TrackingEvent(otherBranch, "LT001", 11, "Accepted elsewhere")), std::logic_error);
    CHECK_EQUAL(std::size_t(2), history.eventCount());
}

TEST_FIXTURE(TrackingFixture, TrackingHistoryFindsLatestEventAtRequestedBranch) {
    history.record(TrackingEvent(branch, "LT001", 0, "Accepted"));
    history.record(TrackingEvent(otherBranch, "LT001", 5, "Checked"));
    history.record(TrackingEvent(otherBranch, "LT001", 6, "Checked again"));
    CHECK_EQUAL(6, history.findLastAtBranch(14).minute());
    CHECK_EQUAL(0, history.findLastAtBranch(13).minute());
    CHECK_THROW(history.findLastAtBranch(99), std::out_of_range);
}

TEST_FIXTURE(TrackingFixture, DeliveryNoticeRequiresDeliveredShipmentAndValidContact) {
    CHECK_THROW(DeliveryNotice(1, branch, "LT001", "contact"), std::logic_error);
    deliver();
    CHECK_THROW(DeliveryNotice(0, branch, "LT001", "contact"), std::invalid_argument);
    CHECK_THROW(DeliveryNotice(1, branch, "LT001", ""), std::invalid_argument);
    CHECK_THROW(DeliveryNotice(1, branch, "LT001", " \t\n"), std::invalid_argument);
    CHECK_THROW(DeliveryNotice(1, branch, "missing", "contact"), PostalException);
}

TEST_FIXTURE(TrackingFixture, DeliveryNoticeFormatsRecipientAndBranchAddress) {
    deliver();
    DeliveryNotice notice(7, branch, "LT001", "recipient@example.test");
    CHECK_EQUAL(7, notice.id());
    CHECK_EQUAL(std::string("LT001"), notice.trackingNumber());
    CHECK_EQUAL(std::string("recipient@example.test"), notice.contact());
    CHECK_EQUAL(13, notice.branchId());
    CHECK(!notice.sent());
    CHECK(!notice.read());
    CHECK(notice.formatMessage().find("Алексей") != std::string::npos);
    CHECK(notice.formatMessage().find(address.formatLabel()) != std::string::npos);
}

TEST_FIXTURE(TrackingFixture, DeliveryNoticeValidatesSentAndReadTransitions) {
    deliver();
    DeliveryNotice notice(1, branch, "LT001", "contact");
    CHECK_THROW(notice.markRead(), std::logic_error);
    notice.markSent();
    CHECK(notice.sent());
    CHECK_THROW(notice.markSent(), std::logic_error);
    notice.markRead();
    CHECK(notice.read());
    CHECK_THROW(notice.markRead(), std::logic_error);
}

TEST_FIXTURE(TrackingFixture, NotificationServiceCreatesSequentialNoticesAndKeepsState) {
    CHECK_EQUAL(13, service.branchId());
    CHECK_EQUAL(std::size_t(0), service.noticeCount());
    deliver();
    int first = service.createNotice(branch, "LT001", "contact1");
    int second = service.createNotice(branch, "LT001", "contact2");
    CHECK_EQUAL(1, first);
    CHECK_EQUAL(2, second);
    CHECK_EQUAL(std::size_t(2), service.noticeCount());
    service.markSent(first);
    service.markRead(first);
    CHECK(service.findNotice(first).sent());
    CHECK(service.findNotice(first).read());
    CHECK(!service.findNotice(second).sent());
}

TEST_FIXTURE(TrackingFixture, NotificationServiceRejectsWrongBranchAndFailedCreationKeepsNextId) {
    CHECK_THROW(service.createNotice(otherBranch, "LT001", "contact"), std::invalid_argument);
    CHECK_THROW(service.createNotice(branch, "LT001", "contact"), std::logic_error);
    deliver();
    CHECK_THROW(service.createNotice(branch, "LT001", ""), std::invalid_argument);
    CHECK_EQUAL(std::size_t(0), service.noticeCount());
    CHECK_EQUAL(1, service.createNotice(branch, "LT001", "contact"));
}

TEST_FIXTURE(TrackingFixture, NotificationServiceReportsMissingNoticeAndRejectsPrematureRead) {
    CHECK_THROW(service.findNotice(1), std::out_of_range);
    CHECK_THROW(service.markSent(1), std::out_of_range);
    CHECK_THROW(service.markRead(1), std::out_of_range);
    deliver();
    int id = service.createNotice(branch, "LT001", "contact");
    CHECK_THROW(service.markRead(id), std::logic_error);
    CHECK(!service.findNotice(id).read());
}
