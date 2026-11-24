/// @file paymentnotfoundexception.h
/// @author Владислав Вертинский
/// @brief Ошибка поиска платежа по ID.

#ifndef PAYMENTNOTFOUNDEXCEPTION_H
#define PAYMENTNOTFOUNDEXCEPTION_H

#include "postalexception.h"

/// @brief Ошибка поиска платежа по ID.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class PaymentNotFoundException : public PostalException {
private:
    int paymentId_; ///< ID платежа.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param paymentId Идентификатор платежа.
    explicit PaymentNotFoundException(int paymentId);

    /// @brief Идентификатор платежа.
    /// @return Идентификатор платежа.
    int paymentId() const;
};

#endif
