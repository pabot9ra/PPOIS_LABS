/// @file capacityexceededexception.h
/// @author Владислав Вертинский
/// @brief Превышение доступной грузоподъёмности в граммах.

#ifndef CAPACITYEXCEEDEDEXCEPTION_H
#define CAPACITYEXCEEDEDEXCEPTION_H

#include "postalexception.h"

/// @brief Превышение доступной грузоподъёмности в граммах.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class CapacityExceededException : public PostalException {
private:
    int requiredWeight_; ///< Требуемый вес из ошибки, г.
    int availableWeight_; ///< Доступный вес из ошибки, г.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param object Название перегруженного объекта для сообщения.
    /// @param requiredWeight Запрошенный вес в граммах.
    /// @param availableWeight Доступный вес в граммах.
    CapacityExceededException(const std::string &object, int requiredWeight, int availableWeight);

    /// @brief Требуемая масса из контекста ошибки в граммах.
    /// @return Требуемая масса из контекста ошибки в граммах.
    int requiredWeight() const;
    /// @brief Доступная масса из контекста ошибки в граммах.
    /// @return Доступная масса из контекста ошибки в граммах.
    int availableWeight() const;
};

#endif
