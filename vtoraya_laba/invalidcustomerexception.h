/// @file invalidcustomerexception.h
/// @author Владислав Вертинский
/// @brief Ошибка данных клиента с сохранением его ID.

#ifndef INVALIDCUSTOMEREXCEPTION_H
#define INVALIDCUSTOMEREXCEPTION_H

#include "postalexception.h"

/// @brief Ошибка данных клиента с сохранением его ID.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class InvalidCustomerException : public PostalException {
private:
    int customerId_; ///< ID клиента из контекста ошибки.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param customerId ID клиента из ошибочного запроса.
    /// @param message Поясняющее сообщение об ошибке.
    InvalidCustomerException(int customerId, const std::string &message);

    /// @brief Идентификатор клиента из контекста ошибки.
    /// @return Идентификатор клиента из контекста ошибки.
    int customerId() const;
};

#endif
