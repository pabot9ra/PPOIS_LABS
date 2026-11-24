/// @file postalclerk.h
/// @author Владислав Вертинский
/// @brief Оператор приёма почтовых отправлений.

#ifndef POSTALCLERK_H
#define POSTALCLERK_H

#include "employee.h"
#include <cstddef>

/// @brief Оператор приёма почтовых отправлений.
/// @details Виртуальная база Employee позволяет совмещать роль с Courier без дублирования данных сотрудника.
class PostalClerk : virtual public Employee {
private:
    std::size_t acceptedCount_; ///< Число успешных приёмов отправлений.

public:
    /// @brief Создаёт оператора без принятых отправлений.
    /// @param id Идентификатор записи.
    /// @param name Непустое имя или название.
    /// @param branchId Идентификатор отделения.
    PostalClerk(int id, const std::string &name, int branchId);

    /// @brief Название почтовой роли сотрудника.
    /// @return Название почтовой роли сотрудника.
    std::string role() const override;
    /// @brief Число успешно принятых оператором отправлений.
    /// @return Число успешно принятых оператором отправлений.
    std::size_t acceptedCount() const;
    /// @brief Принимает отправление в свою смену и увеличивает счётчик только после успеха.
    /// @param branch Отделение, с которым связана операция.
    /// @param shipment Отправление, над которым выполняется операция.
    void acceptShipment(PostalBranch &branch, const Shipment &shipment);
};

#endif
