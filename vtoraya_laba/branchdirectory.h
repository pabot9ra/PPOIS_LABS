/// @file branchdirectory.h
/// @author Владислав Вертинский
/// @brief Справочник отделений и их отправлений.

#ifndef BRANCHDIRECTORY_H
#define BRANCHDIRECTORY_H

#include "postalbranch.h"
#include <cstddef>
#include <map>
#include <memory>

/// @brief Справочник отделений и их отправлений.
/// @details Владеет отделениями через unique_ptr; не допускает одинаковые трек-номера в разных отделениях.
class BranchDirectory {
private:
    std::map<int, std::unique_ptr<PostalBranch>> branches_; ///< Отделения, принадлежащие справочнику.

    /// @brief Находит запись отделения для внутреннего изменения.
    /// @param id Идентификатор записи.
    /// @return Находит запись отделения для внутреннего изменения.
    PostalBranch &findMutable(int id);

public:
    /// @brief Число зарегистрированных записей.
    /// @return Число зарегистрированных записей.
    std::size_t size() const;
    /// @brief Проверяет наличие записи по идентификатору.
    /// @param id Идентификатор записи.
    /// @return true, если запись найдена; иначе false.
    bool contains(int id) const;
    /// @brief Регистрирует запись с уникальным ID.
    /// @param id Идентификатор записи.
    /// @param address Почтовый адрес, сохраняемый копией.
    void add(int id, const PostalAddress &address);
    /// @brief Находит зарегистрированную запись по ID.
    /// @param id Идентификатор записи.
    /// @return Ссылка на запись с указанным ID.
    /// @note Ссылка сохраняется до удаления отделения или уничтожения справочника.
    /// @throws BranchNotFoundException ID отделения не зарегистрирован.
    const PostalBranch &find(int id) const;
    /// @brief Удаляет зарегистрированную запись по ID.
    /// @param id Идентификатор записи.
    void remove(int id);
    /// @brief Число учтённых отправлений.
    /// @return Число учтённых отправлений.
    std::size_t shipmentCount() const;
    /// @brief Проверяет наличие трек-номера среди сохранённых отправлений.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return true, если отправление зарегистрировано; иначе false.
    bool containsShipment(const std::string &trackingNumber) const;
    /// @brief Находит ID отделения, содержащего трек-номер.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return Место учёта или хранения найденного отправления.
    int locateShipment(const std::string &trackingNumber) const;
    /// @brief Принимает отправление с проверкой ограничений текущего объекта.
    /// @param branchId Идентификатор отделения.
    /// @param shipment Отправление, над которым выполняется операция.
    void acceptShipment(int branchId, const Shipment &shipment);
    /// @brief Переводит сохранённое отправление в состояние «в пути».
    /// @param branchId Идентификатор отделения.
    /// @param trackingNumber Трек-номер без пробельных символов.
    void dispatchShipment(int branchId, const std::string &trackingNumber);
    /// @brief Отмечает доставку сохранённого отправления.
    /// @param branchId Идентификатор отделения.
    /// @param trackingNumber Трек-номер без пробельных символов.
    void deliverShipment(int branchId, const std::string &trackingNumber);
};

#endif
