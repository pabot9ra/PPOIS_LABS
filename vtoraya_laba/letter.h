/// @file letter.h
/// @author Владислав Вертинский
/// @brief Письмо с оплатой за грамм.

#ifndef LETTER_H
#define LETTER_H

#include "shipment.h"

/// @brief Письмо с оплатой за грамм.
/// @details Вес и тариф положительны. Стоимость в копейках проверяется на переполнение при создании.
class Letter : public Shipment {
private:
    int weightGrams_; ///< Вес содержимого или записи, г.
    int pricePerGram_; ///< Тариф за грамм, коп.

public:
    /// @brief Создаёт письмо с весом и тарифом.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param sender Отправитель; сохраняется копией.
    /// @param recipient Получатель; сохраняется копией.
    /// @param weightGrams Положительный вес вложения в граммах.
    /// @param pricePerGram Положительный тариф за грамм в копейках.
    /// @throws std::invalid_argument Вес, тариф или параметры услуги недопустимы.
    /// @throws std::overflow_error Итоговая цена не помещается в int.
    Letter(const std::string &trackingNumber, const Customer &sender,
           const Customer &recipient, int weightGrams, int pricePerGram);

    /// @brief Масса содержимого в граммах.
    /// @return Масса содержимого в граммах.
    int weightGrams() const override;
    /// @brief Тариф в копейках за грамм.
    /// @return Тариф в копейках за грамм.
    int pricePerGram() const;
    /// @brief Рассчитывает стоимость пересылки в копейках.
    /// @return Стоимость пересылки в копейках.
    int calculatePrice() const override;
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    std::unique_ptr<Shipment> clone() const override;
};

#endif
