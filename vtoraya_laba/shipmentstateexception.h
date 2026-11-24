/// @file shipmentstateexception.h
/// @author Владислав Вертинский
/// @brief Недопустимая операция с учётом текущего статуса отправления.

#ifndef SHIPMENTSTATEEXCEPTION_H
#define SHIPMENTSTATEEXCEPTION_H

#include "postalexception.h"
#include "shipment.h"

/// @brief Недопустимая операция с учётом текущего статуса отправления.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class ShipmentStateException : public PostalException {
private:
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    ShipmentStatus currentStatus_; ///< Статус в момент отказа.
    std::string operation_; ///< Отклонённая операция.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param currentStatus Статус отправления в момент отказа.
    /// @param operation Название операции, которую не удалось выполнить.
    ShipmentStateException(const std::string &trackingNumber, ShipmentStatus currentStatus, const std::string &operation);

    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Статус отправления в момент отказа.
    /// @return Статус отправления в момент отказа.
    ShipmentStatus currentStatus() const;
    /// @brief Название отклонённой операции.
    /// @return Название отклонённой операции.
    const std::string &operation() const;
};

#endif
