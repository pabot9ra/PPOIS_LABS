/// @file damageclaim.h
/// @author Владислав Вертинский
/// @brief Заявка на компенсацию повреждения застрахованной посылки.

#ifndef DAMAGECLAIM_H
#define DAMAGECLAIM_H

#include "customerrequest.h"
#include "postalbranch.h"

/// @brief Заявка на компенсацию повреждения застрахованной посылки.
/// @details Одобрение возможно после доставки. Остаток покрытия проверяется повторно при исполнении; реальная выплата не производится.
class DamageClaim : public CustomerRequest {
private:
    int requestedAmount_; ///< Запрошенная сумма, коп.
    int settledAmount_; ///< Учтённая компенсация, коп.

protected:
    /// @brief Проверяет вид посылки, доставку и остаток страхового покрытия.
    /// @param shipment Отправление, над которым выполняется операция.
    void validateEligibility(const Shipment &shipment) const override;

public:
    /// @brief Создаёт открытую страховую претензию.
    /// @param id Идентификатор записи.
    /// @param applicant Заявитель: отправитель или получатель указанного отправления.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param openedMinute Неотрицательная минута создания заявки.
    /// @param reason Непустое пояснение причины.
    /// @param requestedAmount Запрошенная сумма в копейках.
    DamageClaim(int id, const Customer &applicant, const Shipment &shipment,
                int openedMinute, const std::string &reason, int requestedAmount);

    /// @brief Запрошенная сумма в копейках.
    /// @return Запрошенная сумма в копейках.
    int requestedAmount() const;
    /// @brief Учтённая при исполнении претензии компенсация в копейках.
    /// @return Учтённая при исполнении претензии компенсация в копейках.
    int settledAmount() const;
    /// @brief Повторно проверяет страховое покрытие и учитывает компенсацию в записи отделения.
    /// @param branch Отделение, с которым связана операция.
    /// @throws std::logic_error Заявка не одобрена или посылка не доставлена.
    /// @throws std::invalid_argument Неверен вид отправления или недостаточно страхового покрытия.
    void settle(PostalBranch &branch);
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    std::unique_ptr<CustomerRequest> clone() const override;
};

#endif
