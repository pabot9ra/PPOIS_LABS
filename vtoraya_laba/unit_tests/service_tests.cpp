#include <UnitTest++/UnitTest++.h>
#include "servicecounter.h"
#include "postalclerk.h"
#include <limits>
#include <stdexcept>
#include <string>

TEST(WorkingHoursHandlesBoundariesAndLunchBreak) {
    WorkingHours hours(540,1080,780,840);
    CHECK_EQUAL(1440, WorkingHours::MINUTES_PER_DAY);
    CHECK_EQUAL(540,hours.opensAt());
    CHECK_EQUAL(1080,hours.closesAt());
    CHECK_EQUAL(780,hours.breakStart());
    CHECK_EQUAL(840,hours.breakEnd());
    CHECK_EQUAL(480,hours.workingMinutes());
    CHECK(!hours.isOpenAt(539));
    CHECK(hours.isOpenAt(540));
    CHECK(!hours.isOpenAt(780));
    CHECK(hours.isOpenAt(840));
    CHECK(!hours.isOpenAt(1080));
    CHECK_THROW(hours.isOpenAt(-1),std::invalid_argument);
    CHECK_THROW(hours.isOpenAt(1440),std::invalid_argument);
}

TEST(WorkingHoursRejectsInvalidIntervalsAndAllDayBreak) {
    CHECK_THROW(WorkingHours(-1,100,20,30),std::invalid_argument);
    CHECK_THROW(WorkingHours(100,100,100,100),std::invalid_argument);
    CHECK_THROW(WorkingHours(0,1441,0,0),std::invalid_argument);
    CHECK_THROW(WorkingHours(100,200,99,120),std::invalid_argument);
    CHECK_THROW(WorkingHours(100,200,150,140),std::invalid_argument);
    CHECK_THROW(WorkingHours(100,200,150,201),std::invalid_argument);
    CHECK_THROW(WorkingHours(100,200,100,200),std::invalid_argument);
}

TEST(WorkingHoursFindsNextOpeningAndAllowsZeroLengthBreak) {
    WorkingHours hours(540,1080,780,840);
    CHECK_EQUAL(540,hours.nextOpenMinute(0));
    CHECK_EQUAL(600,hours.nextOpenMinute(600));
    CHECK_EQUAL(840,hours.nextOpenMinute(800));
    CHECK_THROW(hours.nextOpenMinute(1080),std::out_of_range);
    CHECK_THROW(hours.nextOpenMinute(1440),std::invalid_argument);
    WorkingHours lateStart(540,1080,540,600);
    CHECK_EQUAL(600,lateStart.nextOpenMinute(500));
    WorkingHours earlyClose(540,1080,1000,1080);
    CHECK_THROW(earlyClose.nextOpenMinute(1050),std::out_of_range);
    WorkingHours noBreak(0,1440,100,100);
    CHECK_EQUAL(1440,noBreak.workingMinutes());
    CHECK(noBreak.canServe(99,2));
    CHECK(noBreak.canServe(1439,1));
}

TEST(WorkingHoursChecksWholeServiceIntervalWithoutOverflow) {
    WorkingHours hours(540,1080,780,840);
    CHECK(hours.canServe(770,10));
    CHECK(!hours.canServe(770,11));
    CHECK(!hours.canServe(800,1));
    CHECK(hours.canServe(1070,10));
    CHECK(!hours.canServe(1070,11));
    CHECK(!hours.canServe(600,std::numeric_limits<int>::max()));
    CHECK_THROW(hours.canServe(600,0),std::invalid_argument);
    CHECK_THROW(hours.canServe(600,-1),std::invalid_argument);
}

struct ServiceFixture {
    PostalAddress address{"Минск","Гикало","9","220013"};
    PostalBranch branch{13,address};
    PostalBranch otherBranch{14,address};
    Customer customer{1,"Владислав",address};
    PostalClerk clerk{10,"Ольга",13};
    PostalClerk otherClerk{11,"Анна",13};
    PostalClerk foreignClerk{12,"Иван",14};
    WorkingHours hours{540,1080,780,840};
    ServiceQueue queue{1,branch};
    ServiceCounter counter{1,branch,hours};
};

TEST_FIXTURE(ServiceFixture, QueueTicketStoresDataAndValidatesTransitions) {
    QueueTicket ticket(7,customer,"Parcel",600);
    CHECK_EQUAL(7,ticket.number());
    CHECK_EQUAL(1,ticket.customer().id());
    CHECK_EQUAL(std::string("Parcel"),ticket.service());
    CHECK_EQUAL(600,ticket.issuedMinute());
    CHECK(ticket.status()==TicketStatus::Waiting);
    CHECK_EQUAL(0,ticket.counterId());
    CHECK_THROW(ticket.complete(),std::logic_error);
    CHECK_THROW(ticket.call(0),std::invalid_argument);
    ticket.call(1);
    CHECK_EQUAL(1,ticket.counterId());
    CHECK(ticket.status()==TicketStatus::Called);
    CHECK_THROW(ticket.call(2),std::logic_error);
    CHECK_THROW(ticket.cancel(),std::logic_error);
    ticket.complete();
    CHECK(ticket.status()==TicketStatus::Served);
    CHECK_THROW(ticket.complete(),std::logic_error);
}

TEST_FIXTURE(ServiceFixture, QueueTicketValidatesInputAndCancellation) {
    CHECK_THROW(QueueTicket(0,customer,"Parcel",600),std::invalid_argument);
    CHECK_THROW(QueueTicket(1,customer,"Parcel",-1),std::invalid_argument);
    CHECK_THROW(QueueTicket(1,customer,"Parcel",1440),std::invalid_argument);
    CHECK_THROW(QueueTicket(1,customer,"",600),std::invalid_argument);
    CHECK_THROW(QueueTicket(1,customer," \t\n",600),std::invalid_argument);
    QueueTicket ticket(1,customer,"Parcel",600);
    ticket.cancel();
    CHECK(ticket.status()==TicketStatus::Cancelled);
    CHECK_THROW(ticket.cancel(),std::logic_error);
    CHECK_THROW(ticket.call(1),std::logic_error);
}

TEST_FIXTURE(ServiceFixture, ServiceQueueIssuesSequentialTicketsChronologically) {
    CHECK_THROW(ServiceQueue(0,branch),std::invalid_argument);
    CHECK_EQUAL(1,queue.id());
    CHECK_EQUAL(13,queue.branchId());
    CHECK(queue.issuingOpen());
    CHECK_EQUAL(std::size_t(0),queue.waitingCount());
    CHECK_EQUAL(1,queue.issue(customer,"Letter",600));
    CHECK_THROW(queue.issue(customer,"Parcel",599),std::invalid_argument);
    CHECK_EQUAL(2,queue.issue(customer,"Parcel",600));
    CHECK_EQUAL(std::size_t(2),queue.waitingCount());
    CHECK_EQUAL(std::string("Letter"),queue.findTicket(1).service());
    CHECK_THROW(queue.findTicket(99),std::out_of_range);
}

TEST_FIXTURE(ServiceFixture, ServiceQueueSkipsCancelledTicketsAndDoesNotCallFutureTickets) {
    int first=queue.issue(customer,"Letter",600);
    int second=queue.issue(customer,"Parcel",610);
    queue.cancelTicket(first);
    CHECK_THROW(queue.callNext(1,605),std::out_of_range);
    CHECK_EQUAL(second,queue.callNext(1,610));
    CHECK_EQUAL(std::size_t(0),queue.waitingCount());
    CHECK_THROW(queue.cancelTicket(second),std::logic_error);
    CHECK_THROW(queue.cancelTicket(99),std::out_of_range);
}

TEST_FIXTURE(ServiceFixture, ServiceQueueProtectsCounterOwnershipAndCompletesCalledTickets) {
    queue.issue(customer,"Letter",600);
    queue.issue(customer,"Parcel",600);
    CHECK_THROW(queue.callNext(0,600),std::invalid_argument);
    CHECK_THROW(queue.callNext(1,-1),std::invalid_argument);
    CHECK_THROW(queue.callNext(1,1440),std::invalid_argument);
    CHECK_EQUAL(1,queue.callNext(1,600));
    CHECK_THROW(queue.callNext(1,600),std::logic_error);
    CHECK_THROW(queue.completeTicket(1,2),std::invalid_argument);
    queue.completeTicket(1,1);
    CHECK_THROW(queue.completeTicket(1,1),std::logic_error);
    CHECK_EQUAL(2,queue.callNext(1,600));
}

TEST_FIXTURE(ServiceFixture, ServiceQueueCanStopIssuingWhileServingExistingTickets) {
    queue.issue(customer,"Letter",600);
    CHECK_THROW(queue.openIssuing(),std::logic_error);
    queue.closeIssuing();
    CHECK(!queue.issuingOpen());
    CHECK_THROW(queue.closeIssuing(),std::logic_error);
    CHECK_THROW(queue.issue(customer,"Parcel",600),std::logic_error);
    CHECK_EQUAL(1,queue.callNext(1,600));
    queue.openIssuing();
    CHECK_EQUAL(2,queue.issue(customer,"Parcel",600));
}

TEST_FIXTURE(ServiceFixture, ServiceCounterStoresConfigurationAndRequiresShift) {
    CHECK_THROW(ServiceCounter(0,branch,hours),std::invalid_argument);
    CHECK_EQUAL(1,counter.id());
    CHECK_EQUAL(13,counter.branchId());
    CHECK_EQUAL(540,counter.hours().opensAt());
    CHECK(!counter.isOpen());
    CHECK_EQUAL(0,counter.employeeId());
    CHECK_EQUAL(0,counter.currentTicket());
    CHECK_THROW(counter.open(clerk,600),std::logic_error);
    foreignClerk.startShift();
    CHECK_THROW(counter.open(foreignClerk,600),std::logic_error);
    clerk.startShift();
    CHECK_THROW(counter.open(clerk,800),std::logic_error);
    counter.open(clerk,600);
    CHECK_EQUAL(10,counter.employeeId());
    CHECK_THROW(counter.open(clerk,600),std::logic_error);
}

TEST_FIXTURE(ServiceFixture, ServiceCounterCallsAndCompletesFifoTickets) {
    queue.issue(customer,"Letter",600);
    queue.issue(customer,"Parcel",600);
    clerk.startShift();
    counter.open(clerk,600);
    CHECK_EQUAL(1,counter.callNext(queue,clerk,600,10));
    CHECK_EQUAL(1,counter.currentTicket());
    CHECK_THROW(counter.callNext(queue,clerk,610,10),std::logic_error);
    CHECK_THROW(counter.close(clerk),std::logic_error);
    counter.finishCurrent(queue,clerk);
    CHECK(queue.findTicket(1).status()==TicketStatus::Served);
    CHECK_EQUAL(0,counter.currentTicket());
    CHECK_EQUAL(2,counter.callNext(queue,clerk,610,10));
    counter.finishCurrent(queue,clerk);
    counter.close(clerk);
    CHECK(!counter.isOpen());
    CHECK_EQUAL(0,counter.employeeId());
}

TEST_FIXTURE(ServiceFixture, ServiceCounterRejectsServiceCrossingBreakOrClosing) {
    queue.issue(customer,"Letter",600);
    clerk.startShift();
    counter.open(clerk,600);
    CHECK_THROW(counter.callNext(queue,clerk,770,11),std::logic_error);
    CHECK_THROW(counter.callNext(queue,clerk,800,5),std::logic_error);
    CHECK_THROW(counter.callNext(queue,clerk,1070,11),std::logic_error);
    CHECK_EQUAL(0,counter.currentTicket());
    CHECK_EQUAL(std::size_t(1),queue.waitingCount());
    CHECK_EQUAL(1,counter.callNext(queue,clerk,770,10));
}

TEST_FIXTURE(ServiceFixture, ServiceCounterRejectsWrongEmployeeBranchAndQueue) {
    ServiceQueue foreignQueue(2,otherBranch);
    ServiceQueue anotherQueue(3,branch);
    queue.issue(customer,"Letter",600);
    clerk.startShift();
    otherClerk.startShift();
    CHECK_THROW(counter.callNext(queue,clerk,600,10),std::logic_error);
    counter.open(clerk,600);
    CHECK_THROW(counter.callNext(queue,otherClerk,600,10),std::logic_error);
    CHECK_THROW(counter.callNext(foreignQueue,clerk,600,10),std::invalid_argument);
    counter.callNext(queue,clerk,600,10);
    CHECK_THROW(counter.finishCurrent(anotherQueue,clerk),std::invalid_argument);
    CHECK_THROW(counter.finishCurrent(foreignQueue,clerk),std::invalid_argument);
    CHECK_THROW(counter.finishCurrent(queue,otherClerk),std::logic_error);
    CHECK_THROW(counter.close(otherClerk),std::logic_error);
    counter.finishCurrent(queue,clerk);
}

TEST_FIXTURE(ServiceFixture, ServiceCounterHandlesEmptyQueueAndUnassignedTicket) {
    clerk.startShift();
    counter.open(clerk,600);
    CHECK_THROW(counter.callNext(queue,clerk,600,10),std::out_of_range);
    CHECK_THROW(counter.finishCurrent(queue,clerk),std::logic_error);
    CHECK_EQUAL(0,counter.currentTicket());
}

TEST_FIXTURE(ServiceFixture, ServiceCounterRequiresEmployeeToRemainOnShift) {
    queue.issue(customer,"Letter",600);
    clerk.startShift();
    counter.open(clerk,600);
    counter.callNext(queue,clerk,600,10);
    clerk.endShift();
    CHECK_THROW(counter.finishCurrent(queue,clerk),std::logic_error);
    clerk.startShift();
    counter.finishCurrent(queue,clerk);
    counter.close(clerk);
}

TEST_FIXTURE(ServiceFixture, ServiceCounterRecognizesTicketCompletedThroughQueueApi) {
    queue.issue(customer,"Letter",600);
    clerk.startShift();
    counter.open(clerk,600);
    counter.callNext(queue,clerk,600,10);
    queue.completeTicket(1,counter.id());
    counter.finishCurrent(queue,clerk);
    CHECK_EQUAL(0,counter.currentTicket());
    counter.close(clerk);
}
