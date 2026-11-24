/// @file postalnetwork.h
/// @author Владислав Вертинский
/// @brief Объединение клиентов, отделений и операций с отправлениями.

#ifndef POSTALNETWORK_H
#define POSTALNETWORK_H

#include "customerdirectory.h"
#include "branchdirectory.h"
#include <cstddef>
#include <string>

/// @brief Объединение клиентов, отделений и операций с отправлениями.
/// @details Участники должны быть зарегистрированы. Поиск и проверка уникальности трек-номера выполняются по всей сети.
class PostalNetwork {
private:
    std::string name_; ///< Имя или название объекта.
    CustomerDirectory customers_; ///< Зарегистрированные клиенты по ID.
    BranchDirectory branches_; ///< Отделения, принадлежащие справочнику.

public:
    /// @brief Создаёт пустую почтовую сеть.
    /// @param name Непустое имя или название.
    explicit PostalNetwork(const std::string &name);

    /// @brief Название почтовой сети.
    /// @return Название почтовой сети.
    const std::string &name() const;
    /// @brief Число зарегистрированных клиентов.
    /// @return Число зарегистрированных клиентов.
    std::size_t customerCount() const;
    /// @brief Число зарегистрированных отделений.
    /// @return Число зарегистрированных отделений.
    std::size_t branchCount() const;
    /// @brief Число учтённых отправлений.
    /// @return Число учтённых отправлений.
    std::size_t shipmentCount() const;
    /// @brief Регистрирует копию клиента с уникальным ID.
    /// @param customer Клиент, данные которого используются в операции.
    void registerCustomer(const Customer &customer);
    /// @brief Создаёт новое отделение с уникальным ID.
    /// @param id Идентификатор записи.
    /// @param address Почтовый адрес, сохраняемый копией.
    void openBranch(int id, const PostalAddress &address);
    /// @brief Данные клиента.
    /// @param id Идентификатор записи.
    /// @return Данные клиента.
    const Customer &customer(int id) const;
    /// @brief Находит отделение сети по ID.
    /// @param id Идентификатор записи.
    /// @return Ссылка на отделение сети.
    const PostalBranch &branch(int id) const;
    /// @brief Меняет адрес в справочнике, не изменяя старые отправления.
    /// @param id Идентификатор записи.
    /// @param address Почтовый адрес, сохраняемый копией.
    void changeCustomerAddress(int id, const PostalAddress &address);
    /// @brief Принимает отправление зарегистрированных клиентов с уникальным по сети трек-номером.
    /// @param branchId Идентификатор отделения.
    /// @param shipment Отправление, над которым выполняется операция.
    void acceptShipment(int branchId, const Shipment &shipment);
    /// @brief Находит ID отделения, в котором зарегистрирован трек-номер.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return ID отделения учёта отправления.
    int shipmentBranchId(const std::string &trackingNumber) const;
    /// @brief Находит сохранённое отправление по трек-номеру.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return Ссылка на сохранённое отправление.
    /// @note Ссылка относится к записи сети и сохраняется до уничтожения сети.
    const Shipment &findShipment(const std::string &trackingNumber) const;
    /// @brief Переводит сохранённое отправление в состояние «в пути».
    /// @param trackingNumber Трек-номер без пробельных символов.
    void dispatchShipment(const std::string &trackingNumber);
    /// @brief Отмечает доставку сохранённого отправления.
    /// @param trackingNumber Трек-номер без пробельных символов.
    void deliverShipment(const std::string &trackingNumber);
};

#endif
