/// @file returnrequest.h
/// @author Владислав Вертинский
/// @brief Заявка отправителя на оформление возвратного ярлыка.

#ifndef RETURNREQUEST_H
#define RETURNREQUEST_H

#include "customerrequest.h"

/// @brief Заявка отправителя на оформление возвратного ярлыка.
/// @details Отзыв недоступен после доставки. Ярлык не изменяет маршрут и статус исходного отправления.
class ReturnRequest : public CustomerRequest {
private:
    PostalAddress returnAddress_; ///< Снимок адреса для возвратного ярлыка.
    int returnFee_; ///< Плата за возвратный ярлык, коп.

protected:
    /// @brief Проверяет, что заявитель — отправитель и отправление ещё не доставлено.
    /// @param shipment Отправление, над которым выполняется операция.
    void validateEligibility(const Shipment &shipment) const override;

public:
    /// @brief Создаёт заявку на возврат по адресу заявителя.
    /// @param id Идентификатор записи.
    /// @param applicant Заявитель: отправитель или получатель указанного отправления.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param openedMinute Неотрицательная минута создания заявки.
    /// @param reason Непустое пояснение причины.
    /// @param returnFee Неотрицательная плата за возвратный ярлык, коп.
    ReturnRequest(int id, const Customer &applicant, const Shipment &shipment,
                  int openedMinute, const std::string &reason, int returnFee);

    /// @brief Адрес отправителя, сохранённый для возвратного ярлыка.
    /// @return Адрес отправителя, сохранённый для возвратного ярлыка.
    const PostalAddress &returnAddress() const;
    /// @brief Плата за возвратный ярлык в копейках.
    /// @return Плата за возвратный ярлык в копейках.
    int returnFee() const;
    /// @brief Выдаёт ярлык после повторной проверки отправления и завершает заявку.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @return Готовый ярлык; успешная выдача переводит заявку в Completed.
    std::string issueLabel(const Shipment &shipment);
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    std::unique_ptr<CustomerRequest> clone() const override;
};

#endif
