/// @file payment.h
/// @author Владислав Вертинский
/// @brief Платёж за отправление с частичными возвратами.

#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

/// @brief Платёж за отправление с частичными возвратами.
/// @details До проведения возврат запрещён. Суммарный возврат не превышает исходную сумму.
class Payment {
private:
    int id_; ///< Идентификатор объекта.
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    int amount_; ///< Сумма операции, коп.
    bool captured_; ///< Проведён ли платёж.
    int refundedAmount_; ///< Уже возвращённая сумма, коп.

public:
    /// @brief Создаёт непроведённый платёж.
    /// @param id Идентификатор записи.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param amount Сумма операции в копейках.
    Payment(int id, const std::string &trackingNumber, int amount);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Сумма в копейках.
    /// @return Сумма в копейках.
    int amount() const;
    /// @brief Признак проведённого платежа.
    /// @return Признак проведённого платежа.
    bool captured() const;
    /// @brief Вычисляет доступную к возврату сумму проведённого платежа.
    /// @return Остаток суммы, доступный к возврату, в копейках.
    int refundableAmount() const;
    /// @brief Проводит платёж однократно.
    void capture();
    /// @brief Учитывает положительную часть возврата без превышения исходной суммы.
    /// @param amount Сумма операции в копейках.
    /// @throws RefundLimitException Сумма неположительна или превышает остаток.
    /// @throws std::logic_error Платёж ещё не проведён.
    void refund(int amount);
};

#endif
