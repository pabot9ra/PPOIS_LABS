/// @file registeredletter.h
/// @author Владислав Вертинский
/// @brief Заказное письмо с регистрационной платой и подтверждением получения.

#ifndef REGISTEREDLETTER_H
#define REGISTEREDLETTER_H

#include "letter.h"

/// @brief Заказное письмо с регистрационной платой и подтверждением получения.
/// @details Подпись фиксируется однократно и только после доставки.
class RegisteredLetter : public Letter {
private:
    int registrationFee_; ///< Регистрационная плата, коп.
    std::string signedBy_; ///< Имя подтвердившего получение.

public:
    /// @brief Создаёт заказное письмо без подписи получателя.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param sender Отправитель; сохраняется копией.
    /// @param recipient Получатель; сохраняется копией.
    /// @param weightGrams Положительный вес вложения в граммах.
    /// @param pricePerGram Положительный тариф за грамм в копейках.
    /// @param registrationFee Неотрицательная регистрационная плата в копейках.
    /// @throws std::invalid_argument Вес, тариф или параметры услуги недопустимы.
    /// @throws std::overflow_error Итоговая цена не помещается в int.
    RegisteredLetter(const std::string &trackingNumber, const Customer &sender,
                     const Customer &recipient, int weightGrams, int pricePerGram,
                     int registrationFee);

    /// @brief Регистрационная доплата в копейках.
    /// @return Регистрационная доплата в копейках.
    int registrationFee() const;
    /// @brief Имя подписавшего получение; пустая строка до подтверждения.
    /// @return Имя подписавшего получение; пустая строка до подтверждения.
    const std::string &signedBy() const;
    /// @brief Фиксирует однократное подтверждение получения заказного письма после доставки.
    /// @param signer Непустое имя подтвердившего получение.
    void confirmReceipt(const std::string &signer);
    /// @brief Рассчитывает стоимость пересылки в копейках.
    /// @return Стоимость пересылки в копейках.
    int calculatePrice() const override;
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    std::unique_ptr<Shipment> clone() const override;
};

#endif
