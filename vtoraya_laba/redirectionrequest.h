/// @file redirectionrequest.h
/// @author Владислав Вертинский
/// @brief Заявка на оформление ярлыка с новым адресом.

#ifndef REDIRECTIONREQUEST_H
#define REDIRECTIONREQUEST_H

#include "customerrequest.h"

/// @brief Заявка на оформление ярлыка с новым адресом.
/// @details Выдача ярлыка завершает заявку, но не меняет адрес исходного отправления и не запускает перевозку.
class RedirectionRequest : public CustomerRequest {
private:
    PostalAddress destination_; ///< Новый адрес для ярлыка.
    int redirectionFee_; ///< Плата за переадресацию, коп.

protected:
    /// @brief Проверяет отсутствие доставки и отличие нового адреса от исходного.
    /// @param shipment Отправление, над которым выполняется операция.
    void validateEligibility(const Shipment &shipment) const override;

public:
    /// @brief Создаёт заявку на переадресацию.
    /// @param id Идентификатор записи.
    /// @param applicant Заявитель: отправитель или получатель указанного отправления.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param openedMinute Неотрицательная минута создания заявки.
    /// @param reason Непустое пояснение причины.
    /// @param destination Новый адрес для ярлыка переадресации.
    /// @param redirectionFee Неотрицательная плата за переадресацию, коп.
    RedirectionRequest(int id, const Customer &applicant, const Shipment &shipment,
                       int openedMinute, const std::string &reason,
                       const PostalAddress &destination, int redirectionFee);

    /// @brief Новый адрес для ярлыка переадресации.
    /// @return Новый адрес для ярлыка переадресации.
    const PostalAddress &destination() const;
    /// @brief Плата за переадресацию в копейках.
    /// @return Плата за переадресацию в копейках.
    int redirectionFee() const;
    /// @brief Выдаёт ярлык после повторной проверки отправления и завершает заявку.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @return Готовый ярлык; успешная выдача переводит заявку в Completed.
    std::string issueLabel(const Shipment &shipment);
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    std::unique_ptr<CustomerRequest> clone() const override;
};

#endif
