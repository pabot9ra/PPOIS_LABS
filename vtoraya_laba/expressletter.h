/// @file expressletter.h
/// @author Владислав Вертинский
/// @brief Срочное письмо со сроком доставки и доплатой.

#ifndef EXPRESSLETTER_H
#define EXPRESSLETTER_H

#include "letter.h"

/// @brief Срочное письмо со сроком доставки и доплатой.
/// @details Просрочка проверяется по переданному числу часов, пока письмо находится в пути; часы не отсчитываются автоматически.
class ExpressLetter : public Letter {
private:
    int deliveryHours_; ///< Срок доставки, часы.
    int expressFee_; ///< Доплата за срочность, коп.

public:
    /// @brief Создаёт срочное письмо и проверяет срок и стоимость.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param sender Отправитель; сохраняется копией.
    /// @param recipient Получатель; сохраняется копией.
    /// @param weightGrams Положительный вес вложения в граммах.
    /// @param pricePerGram Положительный тариф за грамм в копейках.
    /// @param deliveryHours Положительный срок доставки в часах.
    /// @param expressFee Неотрицательная доплата за срочность в копейках.
    /// @throws std::invalid_argument Вес, тариф или параметры услуги недопустимы.
    /// @throws std::overflow_error Итоговая цена не помещается в int.
    ExpressLetter(const std::string &trackingNumber, const Customer &sender,
                  const Customer &recipient, int weightGrams, int pricePerGram,
                  int deliveryHours, int expressFee);

    /// @brief Срок доставки в часах.
    /// @return Срок доставки в часах.
    int deliveryHours() const;
    /// @brief Доплата за срочность в копейках.
    /// @return Доплата за срочность в копейках.
    int expressFee() const;
    /// @brief Проверяет превышение срока только для находящегося в пути срочного письма.
    /// @param hoursInTransit Неотрицательное число часов в пути, переданное вызывающим кодом.
    /// @return true, если письмо в пути и переданное время превышает срок; иначе false.
    bool isOverdue(int hoursInTransit) const;
    /// @brief Рассчитывает стоимость пересылки в копейках.
    /// @return Стоимость пересылки в копейках.
    int calculatePrice() const override;
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    std::unique_ptr<Shipment> clone() const override;
};

#endif
