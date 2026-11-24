/// @file courier.h
/// @author Владислав Вертинский
/// @brief Курьер, отправляющий и доставляющий отправления.

#ifndef COURIER_H
#define COURIER_H

#include "employee.h"
#include <cstddef>

/// @brief Курьер, отправляющий и доставляющий отправления.
/// @details Работает только в свою смену и в назначенном отделении. Счётчики растут после успешной операции.
class Courier : virtual public Employee {
private:
    std::size_t dispatchedCount_; ///< Число успешных отправок.
    std::size_t deliveredCount_; ///< Число успешных доставок.

public:
    /// @brief Создаёт курьера без выполненных операций.
    /// @param id Идентификатор записи.
    /// @param name Непустое имя или название.
    /// @param branchId Идентификатор отделения.
    Courier(int id, const std::string &name, int branchId);

    /// @brief Название почтовой роли сотрудника.
    /// @return Название почтовой роли сотрудника.
    std::string role() const override;
    /// @brief Число успешных отправок курьером.
    /// @return Число успешных отправок курьером.
    std::size_t dispatchedCount() const;
    /// @brief Число успешных отметок доставки курьером.
    /// @return Число успешных отметок доставки курьером.
    std::size_t deliveredCount() const;
    /// @brief Переводит сохранённое отправление в состояние «в пути».
    /// @param branch Отделение, с которым связана операция.
    /// @param trackingNumber Трек-номер без пробельных символов.
    void dispatchShipment(PostalBranch &branch, const std::string &trackingNumber);
    /// @brief Отмечает доставку сохранённого отправления.
    /// @param branch Отделение, с которым связана операция.
    /// @param trackingNumber Трек-номер без пробельных символов.
    void deliverShipment(PostalBranch &branch, const std::string &trackingNumber);
};

#endif
