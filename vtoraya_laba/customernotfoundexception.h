/// @file customernotfoundexception.h
/// @author Владислав Вертинский
/// @brief Ошибка поиска клиента по ID.

#ifndef CUSTOMERNOTFOUNDEXCEPTION_H
#define CUSTOMERNOTFOUNDEXCEPTION_H

#include "postalexception.h"

/// @brief Ошибка поиска клиента по ID.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class CustomerNotFoundException : public PostalException {
private:
    int customerId_; ///< ID клиента из контекста ошибки.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param customerId ID клиента из ошибочного запроса.
    explicit CustomerNotFoundException(int customerId);

    /// @brief Идентификатор клиента из контекста ошибки.
    /// @return Идентификатор клиента из контекста ошибки.
    int customerId() const;
};

#endif
