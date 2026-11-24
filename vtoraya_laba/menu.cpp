#include "menu.h"
#include "demos.h"
#include "postalnetwork.h"
#include "letter.h"
#include "parcel.h"
#include "registeredletter.h"
#include "expressletter.h"
#include "fragileparcel.h"
#include "insuredparcel.h"
#include <istream>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
std::string readText(std::istream &input, std::ostream &output, const std::string &prompt) {
    output << prompt;
    std::string value;
    if(!std::getline(input, value)){
        throw std::runtime_error("Ввод завершён");
    }
    return value;
}

int readInt(std::istream &input, std::ostream &output, const std::string &prompt) {
    std::string text = readText(input, output, prompt);
    std::istringstream parser(text);
    int value;
    if(!(parser >> value)){
        throw std::invalid_argument("Введите целое число");
    }
    parser >> std::ws;
    if(!parser.eof()){
        throw std::invalid_argument("После числа не должно быть других символов");
    }
    return value;
}

PostalAddress readAddress(std::istream &input, std::ostream &output) {
    std::string city = readText(input, output, "Город: ");
    std::string street = readText(input, output, "Улица: ");
    std::string house = readText(input, output, "Дом: ");
    std::string postalCode = readText(input, output, "Индекс (6 цифр): ");
    return PostalAddress(city, street, house, postalCode);
}

void addCustomer(PostalNetwork &network, std::istream &input, std::ostream &output) {
    int id = readInt(input, output, "ID клиента: ");
    std::string name = readText(input, output, "Имя: ");
    PostalAddress address = readAddress(input, output);
    network.registerCustomer(Customer(id, name, address));
    output << "Клиент зарегистрирован\n";
}

void addBranch(PostalNetwork &network, std::istream &input, std::ostream &output) {
    int id = readInt(input, output, "ID отделения: ");
    PostalAddress address = readAddress(input, output);
    network.openBranch(id, address);
    output << "Отделение зарегистрировано\n";
}

std::unique_ptr<Shipment> readLetter(int kind, const std::string &number, const Customer &sender,
                                    const Customer &recipient, int weight,
                                    std::istream &input, std::ostream &output) {
    int rate = readInt(input, output, "Тариф за грамм, коп.: ");
    if(kind == 1){
        return std::make_unique<Letter>(number, sender, recipient, weight, rate);
    }
    if(kind == 3){
        int fee = readInt(input, output, "Плата за регистрацию, коп.: ");
        return std::make_unique<RegisteredLetter>(number, sender, recipient, weight, rate, fee);
    }
    int hours = readInt(input, output, "Срок доставки, часов: ");
    int fee = readInt(input, output, "Доплата за срочность, коп.: ");
    return std::make_unique<ExpressLetter>(number, sender, recipient, weight, rate, hours, fee);
}

std::unique_ptr<Shipment> readParcel(int kind, const std::string &number, const Customer &sender,
                                    const Customer &recipient, int weight,
                                    std::istream &input, std::ostream &output) {
    int base = readInt(input, output, "Базовая плата, коп.: ");
    int rate = readInt(input, output, "Тариф за начатый килограмм, коп.: ");
    if(kind == 2){
        return std::make_unique<Parcel>(number, sender, recipient, weight, base, rate);
    }
    if(kind == 5){
        int fee = readInt(input, output, "Доплата за упаковку, коп.: ");
        int upright = readInt(input, output, "Хранить вертикально (0 или 1): ");
        if(upright != 0 && upright != 1){
            throw std::invalid_argument("Выберите 0 или 1");
        }
        return std::make_unique<FragileParcel>(number, sender, recipient, weight, base, rate, fee, upright == 1);
    }
    int value = readInt(input, output, "Объявленная ценность, коп.: ");
    int fee = readInt(input, output, "Страховая плата, коп.: ");
    return std::make_unique<InsuredParcel>(number, sender, recipient, weight, base, rate, value, fee);
}

std::unique_ptr<Shipment> readShipment(const PostalNetwork &network, std::istream &input, std::ostream &output) {
    output << "1 — письмо, 2 — посылка, 3 — заказное письмо, 4 — срочное письмо,\n"
           << "5 — хрупкая посылка, 6 — застрахованная посылка\n";
    int kind = readInt(input, output, "Вид: ");
    if(kind < 1 || kind > 6){
        throw std::invalid_argument("Неизвестный вид отправления");
    }
    std::string number = readText(input, output, "Трек-номер: ");
    int senderId = readInt(input, output, "ID отправителя: ");
    int recipientId = readInt(input, output, "ID получателя: ");
    const Customer &sender = network.customer(senderId);
    const Customer &recipient = network.customer(recipientId);
    int weight = readInt(input, output, "Вес, г: ");
    if(kind == 1 || kind == 3 || kind == 4){
        return readLetter(kind, number, sender, recipient, weight, input, output);
    }
    return readParcel(kind, number, sender, recipient, weight, input, output);
}

void addShipment(PostalNetwork &network, std::istream &input, std::ostream &output) {
    int branchId = readInt(input, output, "ID отделения приёма: ");
    network.branch(branchId);
    auto shipment = readShipment(network, input, output);
    network.acceptShipment(branchId, *shipment);
    output << "Отправление принято. Стоимость: " << shipment->calculatePrice() << " коп.\n";
}

std::string statusName(ShipmentStatus status) {
    switch(status){
        case ShipmentStatus::Accepted: return "Принято";
        case ShipmentStatus::InTransit: return "В пути";
        case ShipmentStatus::Delivered: return "Доставлено";
    }
    throw std::logic_error("Неизвестный статус отправления");
}

void showShipment(const PostalNetwork &network, std::istream &input, std::ostream &output) {
    std::string number = readText(input, output, "Трек-номер: ");
    const Shipment &shipment = network.findShipment(number);
    output << "Трек: " << shipment.trackingNumber() << "\nСтатус: " << statusName(shipment.status())
           << "\nОтделение учёта: " << network.shipmentBranchId(number)
           << "\nОтправитель: " << shipment.sender().name()
           << "\nПолучатель: " << shipment.recipient().name()
           << "\nАдрес: " << shipment.recipient().address().formatLabel()
           << "\nВес: " << shipment.weightGrams() << " г\nСтоимость: " << shipment.calculatePrice() << " коп.\n";
}

void dispatchShipment(PostalNetwork &network, std::istream &input, std::ostream &output) {
    std::string number = readText(input, output, "Трек-номер: ");
    network.dispatchShipment(number);
    output << "Отправление в пути\n";
}

void deliverShipment(PostalNetwork &network, std::istream &input, std::ostream &output) {
    std::string number = readText(input, output, "Трек-номер: ");
    network.deliverShipment(number);
    output << "Доставка отмечена\n";
}

void changeAddress(PostalNetwork &network, std::istream &input, std::ostream &output) {
    int id = readInt(input, output, "ID клиента: ");
    network.customer(id);
    PostalAddress address = readAddress(input, output);
    network.changeCustomerAddress(id, address);
    output << "Адрес клиента обновлён. Адреса ранее созданных отправлений сохранены\n";
}

void showSummary(const PostalNetwork &network, std::ostream &output) {
    output << "Клиентов: " << network.customerCount() << "\nОтделений: " << network.branchCount()
           << "\nОтправлений: " << network.shipmentCount() << '\n';
}

void examples(std::istream &input, std::ostream &output) {
    output << "Примеры используют отдельные демонстрационные данные:\n"
           << "1 — оплата и возврат, 2 — упаковка, 3 — хранение и перевозка,\n"
           << "4 — заявки и уведомления, 5 — очередь и сотрудники, 0 — все примеры\n";
    int number = readInt(input, output, "Пример: ");
    if(number == 0){
        runDemonstrations(output);
    } else {
        runDemonstration(number, output);
    }
}

void execute(int choice, PostalNetwork &network, std::istream &input, std::ostream &output) {
    switch(choice){
        case 1: addCustomer(network, input, output); break;
        case 2: addBranch(network, input, output); break;
        case 3: addShipment(network, input, output); break;
        case 4: showShipment(network, input, output); break;
        case 5: dispatchShipment(network, input, output); break;
        case 6: deliverShipment(network, input, output); break;
        case 7: changeAddress(network, input, output); break;
        case 8: showSummary(network, output); break;
        case 9: examples(input, output); break;
        default: throw std::invalid_argument("Нет такого пункта меню");
    }
}

void printMenu(std::ostream &output) {
    output << "\nПОЧТА\n1 — добавить клиента\n2 — добавить отделение\n3 — принять отправление\n"
           << "4 — найти отправление\n5 — отправить\n6 — отметить доставку\n"
           << "7 — изменить адрес клиента\n8 — сводка\n9 — примеры остальных операций\n0 — выход\n";
}
}

int runMenu(std::istream &input, std::ostream &output) {
    PostalNetwork network("Учебная почта");
    while(true){
        printMenu(output);
        try {
            int choice = readInt(input, output, "> ");
            if(choice == 0){
                return 0;
            }
            execute(choice, network, input, output);
        } catch(const std::exception &error) {
            if(input.eof()){
                return 0;
            }
            output << "Ошибка: " << error.what() << '\n';
        }
    }
}
