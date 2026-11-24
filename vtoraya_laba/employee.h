/// @file employee.h
/// @author Владислав Вертинский
/// @brief Абстрактный сотрудник с назначением и состоянием смены.

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "postalbranch.h"
#include <string>

/// @brief Абстрактный сотрудник с назначением и состоянием смены.
/// @details Перевод между отделениями разрешён только вне смены. Присваивание через базовый тип запрещено.
class Employee {
private:
    int id_; ///< Идентификатор объекта.
    std::string name_; ///< Имя или название объекта.
    int branchId_; ///< Идентификатор отделения.
    bool onShift_; ///< Находится ли сотрудник на смене.

protected:
    /// @brief Проверяет смену сотрудника и соответствие отделению.
    /// @param branch Отделение, с которым связана операция.
    void ensureCanWork(const PostalBranch &branch) const;

public:
    /// @brief Создаёт сотрудника вне смены.
    /// @param id Идентификатор записи.
    /// @param name Непустое имя или название.
    /// @param branchId Идентификатор отделения.
    Employee(int id, const std::string &name, int branchId);
    /// @brief Освобождает объект при удалении через базовый указатель.
    virtual ~Employee() = default;
    /// @brief Запрещает частичное присваивание через базовый тип.
    /// @param other Другой объект для сравнения или копирования.
    Employee &operator=(const Employee &other) = delete;

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Имя сотрудника.
    /// @return Имя сотрудника.
    const std::string &name() const;
    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Признак текущей смены сотрудника.
    /// @return Признак текущей смены сотрудника.
    bool onShift() const;
    /// @brief Название почтовой роли сотрудника.
    /// @return Название почтовой роли сотрудника.
    virtual std::string role() const = 0;

    /// @brief Начинает смену; повторное открытие запрещено.
    virtual void startShift();
    /// @brief Завершает текущую смену.
    virtual void endShift();
    /// @brief Переводит сотрудника в отделение между сменами.
    /// @param branchId Идентификатор отделения.
    virtual void assignToBranch(int branchId);
};

#endif
