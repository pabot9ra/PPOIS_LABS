/// @file universalemployee.h
/// @author Владислав Вертинский
/// @brief Сотрудник, совмещающий работу оператора и курьера.

#ifndef UNIVERSALEMPLOYEE_H
#define UNIVERSALEMPLOYEE_H

#include "postalclerk.h"
#include "courier.h"

/// @brief Сотрудник, совмещающий работу оператора и курьера.
/// @details Два пути наследования используют одну виртуальную базу Employee, поэтому ID и состояние смены общие.
class UniversalEmployee : public PostalClerk, public Courier {
public:
    /// @brief Создаёт сотрудника с двумя почтовыми ролями.
    /// @param id Идентификатор записи.
    /// @param name Непустое имя или название.
    /// @param branchId Идентификатор отделения.
    UniversalEmployee(int id, const std::string &name, int branchId);

    /// @brief Название почтовой роли сотрудника.
    /// @return Название почтовой роли сотрудника.
    std::string role() const override;
};

#endif
