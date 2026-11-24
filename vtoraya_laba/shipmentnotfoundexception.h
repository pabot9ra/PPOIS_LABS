/// @file shipmentnotfoundexception.h
/// @author Владислав Вертинский
/// @brief Ошибка поиска отправления с сохранением трек-номера.

#ifndef SHIPMENTNOTFOUNDEXCEPTION_H
#define SHIPMENTNOTFOUNDEXCEPTION_H

#include "postalexception.h"

/// @brief Ошибка поиска отправления с сохранением трек-номера.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class ShipmentNotFoundException : public PostalException {
private:
    std::string trackingNumber_; ///< Трек-номер связанного отправления.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param trackingNumber Трек-номер без пробельных символов.
    explicit ShipmentNotFoundException(const std::string &trackingNumber);

    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
};

#endif
