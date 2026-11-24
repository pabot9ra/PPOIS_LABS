/// @file receipt.h
/// @author Владислав Вертинский
/// @brief Неизменяемый снимок данных проведённого платежа.

#ifndef RECEIPT_H
#define RECEIPT_H

#include "payment.h"
#include <string>

/// @brief Неизменяемый снимок данных проведённого платежа.
/// @details Последующий возврат не меняет сумму исходного чека.
class Receipt {
private:
    int registerId_; ///< ID кассы.
    int paymentId_; ///< ID платежа.
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    int amount_; ///< Сумма операции, коп.

public:
    /// @brief Создаёт чек по проведённому платежу.
    /// @param registerId Положительный идентификатор кассы.
    /// @param payment Проведённый платёж, используемый для снимка чека.
    Receipt(int registerId, const Payment &payment);

    /// @brief Идентификатор кассы.
    /// @return Идентификатор кассы.
    int registerId() const;
    /// @brief Идентификатор платежа.
    /// @return Идентификатор платежа.
    int paymentId() const;
    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Сумма в копейках.
    /// @return Сумма в копейках.
    int amount() const;
    /// @brief Формирует строку чека с кассой, платежом, трек-номером и суммой в копейках.
    /// @return Текстовое представление записи.
    std::string format() const;
};

#endif
