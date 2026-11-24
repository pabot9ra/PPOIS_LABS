/// @file refundlimitexception.h
/// @author Владислав Вертинский
/// @brief Недопустимый возврат с указанием запрошенной и доступной суммы.

#ifndef REFUNDLIMITEXCEPTION_H
#define REFUNDLIMITEXCEPTION_H

#include "postalexception.h"

/// @brief Недопустимый возврат с указанием запрошенной и доступной суммы.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class RefundLimitException : public PostalException {
private:
    int requestedAmount_; ///< Запрошенная сумма, коп.
    int availableAmount_; ///< Доступная сумма из контекста ошибки, коп.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param requestedAmount Запрошенная сумма из ошибки, в том числе недопустимая.
    /// @param availableAmount Допустимый остаток суммы, коп.
    RefundLimitException(int requestedAmount, int availableAmount);

    /// @brief Запрошенная сумма в копейках.
    /// @return Запрошенная сумма в копейках.
    int requestedAmount() const;
    /// @brief Доступная к возврату сумма из контекста ошибки в копейках.
    /// @return Доступная к возврату сумма из контекста ошибки в копейках.
    int availableAmount() const;
};

#endif
