/// @file duplicateshipmentexception.h
/// @author Владислав Вертинский
/// @brief Повторная регистрация уже известного трек-номера.

#ifndef DUPLICATESHIPMENTEXCEPTION_H
#define DUPLICATESHIPMENTEXCEPTION_H

#include "postalexception.h"

/// @brief Повторная регистрация уже известного трек-номера.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class DuplicateShipmentException : public PostalException {
private:
    std::string trackingNumber_; ///< Трек-номер связанного отправления.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param trackingNumber Трек-номер без пробельных символов.
    explicit DuplicateShipmentException(const std::string &trackingNumber);

    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
};

#endif
