/// @file refund.h
/// @author Владислав Вертинский
/// @brief Документ частичного или полного возврата платежа.

#ifndef REFUND_H
#define REFUND_H

#include <string>

/// @brief Документ частичного или полного возврата платежа.
/// @details Хранит ID, ссылочный ID платежа, сумму в копейках и непустую причину.
class Refund {
private:
    int id_; ///< Идентификатор объекта.
    int paymentId_; ///< ID платежа.
    int amount_; ///< Сумма операции, коп.
    std::string reason_; ///< Причина операции.

public:
    /// @brief Создаёт и проверяет документ возврата.
    /// @param id Идентификатор записи.
    /// @param paymentId Идентификатор платежа.
    /// @param amount Сумма операции в копейках.
    /// @param reason Непустое пояснение причины.
    Refund(int id, int paymentId, int amount, const std::string &reason);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Идентификатор платежа.
    /// @return Идентификатор платежа.
    int paymentId() const;
    /// @brief Сумма в копейках.
    /// @return Сумма в копейках.
    int amount() const;
    /// @brief Причина операции.
    /// @return Причина операции.
    const std::string &reason() const;
};

#endif
