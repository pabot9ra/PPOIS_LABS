#include "demos.h"
#include "cashier.h"
#include "branchmanager.h"
#include "envelope.h"
#include "box.h"
#include "fragileparcel.h"
#include "registeredletter.h"
#include "insuredparcel.h"
#include "sortingcenter.h"
#include "vehicle.h"
#include "trackinghistory.h"
#include "notificationservice.h"
#include "requestregistry.h"
#include "returnrequest.h"
#include "redirectionrequest.h"
#include "damageclaim.h"
#include "universalemployee.h"
#include "servicecounter.h"
#include <ostream>
#include <stdexcept>

namespace {
struct ExampleData {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    PostalAddress otherAddress{"Брест", "Советская", "1", "224000"};
    PostalBranch branch{13, address};
    PostalBranch destination{14, otherAddress};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"DEMO-LT", sender, recipient, 20, 5};
    InsuredParcel parcel{"DEMO-PK", sender, recipient, 1500, 200, 100, 10000, 100};
};

void payments(std::ostream &output) {
    ExampleData data;
    BranchManager manager(10, "Анна", 13);
    Cashier cashier(11, "Иван", 13, 100);
    manager.startShift();
    manager.startEmployeeShift(data.branch, cashier);
    Receipt receipt = cashier.acceptPayment(data.branch, 1, data.letter);
    output << "\nОПЛАТА И ВОЗВРАТ\n" << receipt.format() << '\n';
    Refund refund(1, 1, 40, "Возврат части платы");
    cashier.processRefund(data.branch, refund);
    output << "Возвращено 40 коп., остаток в кассе: " << cashier.balance() << " коп.\n";
    manager.endEmployeeShift(data.branch, cashier);
    manager.endShift();
}

void packaging(std::ostream &output) {
    ExampleData data;
    Envelope envelope(Dimensions(220, 110, 10), 100, 20, true, true);
    envelope.pack(data.letter, Dimensions(200, 100, 5));
    envelope.seal();
    Box box(Dimensions(200, 200, 200), 2000, 100, 10, "Cardboard");
    box.pack(data.parcel, Dimensions(150, 150, 150));
    box.seal();
    output << "\nУПАКОВКА\n" << envelope.label() << "\n" << box.label()
           << "\nОба отправления упакованы и запечатаны\n";
}

MailBag sortParcel(ExampleData &data) {
    StorageZone zone("A", 13);
    zone.addCell(StorageCell("C1", 13, Dimensions(200, 200, 200), 2000));
    SortingCenter center(1, data.branch, 5000);
    center.addZone(zone);
    center.openShift();
    center.receiveShipment(data.parcel, Dimensions(150, 150, 150));
    center.createBag("DEMO-BAG", 14, 2000);
    center.sortIntoBag(data.parcel, "DEMO-BAG");
    center.sealBag("DEMO-BAG");
    MailBag bag = center.takeBag("DEMO-BAG");
    center.closeShift();
    return bag;
}

void transportation(std::ostream &output) {
    ExampleData data;
    MailBag bag = sortParcel(data);
    Route route(1, "Минск — Брест");
    route.addStop(RouteStop(data.branch, 0, 0));
    route.addStop(RouteStop(data.destination, 350, 300));
    Vehicle vehicle("1234 AB-7", 5000, route);
    vehicle.loadBag(bag);
    vehicle.depart();
    vehicle.arriveAt(14);
    MailBag unloaded = vehicle.unloadBag("DEMO-BAG");
    output << "\nХРАНЕНИЕ И ПЕРЕВОЗКА\nМешок " << unloaded.id() << ", вес " << unloaded.weightGrams()
           << " г, выгружен в отделении " << vehicle.currentBranchId() << '\n';
}

void returnLabels(ExampleData &data, std::ostream &output) {
    data.branch.acceptShipment(data.letter);
    RequestRegistry registry(data.branch);
    ReturnRequest recall(1, data.sender, data.letter, 0, "Возврат отправителю", 50);
    RedirectionRequest redirection(2, data.recipient, data.letter, 0, "Новый адрес", data.otherAddress, 70);
    registry.submit(data.branch, recall);
    registry.submit(data.branch, redirection);
    registry.approve(1, data.branch);
    registry.approve(2, data.branch);
    output << registry.issueReturnLabel(1, data.branch) << '\n'
           << registry.issueRedirectionLabel(2, data.branch) << '\n';
}

void trackDelivery(ExampleData &data, std::ostream &output) {
    data.branch.acceptShipment(data.parcel);
    TrackingHistory history(data.parcel);
    history.record(TrackingEvent(data.branch, "DEMO-PK", 0, "Принято"));
    data.branch.dispatchShipment("DEMO-PK");
    history.record(TrackingEvent(data.branch, "DEMO-PK", 10, "Отправлено"));
    data.branch.deliverShipment("DEMO-PK");
    history.record(TrackingEvent(data.branch, "DEMO-PK", 60, "Доставлено"));
    output << "Событий в истории: " << history.eventCount() << ", прошло " << history.elapsedMinutes() << " мин.\n";
}

void claimAndNotice(ExampleData &data, std::ostream &output) {
    RequestRegistry registry(data.branch);
    DamageClaim claim(3, data.recipient, data.parcel, 60, "Повреждение вложения", 2500);
    registry.submit(data.branch, claim);
    registry.approve(3, data.branch);
    registry.settleDamageClaim(3, data.branch);
    output << "Учтена страховая компенсация 2500 коп.\n";
    NotificationService notices(data.branch);
    int id = notices.createNotice(data.branch, "DEMO-PK", "demo@example.test");
    notices.markSent(id);
    notices.markRead(id);
    output << notices.findNotice(id).formatMessage() << "\nУведомление отмечено прочитанным\n";
}

void requests(std::ostream &output) {
    ExampleData data;
    output << "\nЗАЯВКИ И УВЕДОМЛЕНИЯ\n";
    returnLabels(data, output);
    trackDelivery(data, output);
    claimAndNotice(data, output);
}

void customerService(std::ostream &output) {
    ExampleData data;
    UniversalEmployee employee(10, "Ольга", 13);
    employee.startShift();
    employee.acceptShipment(data.branch, data.letter);
    ServiceQueue queue(1, data.branch);
    queue.issue(data.sender, "Отправка письма", 600);
    ServiceCounter counter(1, data.branch, WorkingHours(540, 1080, 780, 840));
    counter.open(employee, 600);
    int number = counter.callNext(queue, employee, 600, 10);
    counter.finishCurrent(queue, employee);
    counter.close(employee);
    employee.dispatchShipment(data.branch, "DEMO-LT");
    employee.deliverShipment(data.branch, "DEMO-LT");
    output << "\nОЧЕРЕДЬ И СОТРУДНИКИ\nОбслужен талон №" << number << ", письмо доставлено\n";
    employee.endShift();
}
}

void runDemonstration(int number, std::ostream &output) {
    switch(number){
        case 1: payments(output); break;
        case 2: packaging(output); break;
        case 3: transportation(output); break;
        case 4: requests(output); break;
        case 5: customerService(output); break;
        default: throw std::invalid_argument("Неизвестный пример");
    }
}

void runDemonstrations(std::ostream &output) {
    output << "Демонстрационные операции. Уведомления моделируются без реальной рассылки.\n";
    for(int number = 1; number <= 5; number++){
        runDemonstration(number, output);
    }
}
