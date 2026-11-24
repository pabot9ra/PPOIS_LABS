/// @file invalidaddressexception.h
/// @author Владислав Вертинский
/// @brief Ошибка адреса с указанием проблемного поля.

#ifndef INVALIDADDRESSEXCEPTION_H
#define INVALIDADDRESSEXCEPTION_H

#include "postalexception.h"

/// @brief Ошибка адреса с указанием проблемного поля.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class InvalidAddressException : public PostalException {
private:
    std::string field_; ///< Проблемное поле адреса.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param field Название проблемного поля адреса.
    /// @param message Поясняющее сообщение об ошибке.
    InvalidAddressException(const std::string &field, const std::string &message);

    /// @brief Название проблемного поля адреса.
    /// @return Название проблемного поля адреса.
    const std::string &field() const;
};

#endif
