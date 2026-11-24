/// @file parcel.h
/// @author Владислав Вертинский
/// @brief Посылка с базовой платой и тарифом за начатый килограмм.

#ifndef PARCEL_H
#define PARCEL_H

#include "shipment.h"

/// @brief Посылка с базовой платой и тарифом за начатый килограмм.
/// @details Каждый неполный килограмм округляется вверх. Суммы заданы в копейках.
class Parcel : public Shipment {
private:
    int weightGrams_; ///< Вес содержимого или записи, г.
    int basePrice_; ///< Базовая плата, коп.
    int pricePerKilogram_; ///< Тариф за начатый килограмм, коп.

public:
    /// @brief Создаёт посылку и проверяет вес, тариф и итоговую стоимость.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param sender Отправитель; сохраняется копией.
    /// @param recipient Получатель; сохраняется копией.
    /// @param weightGrams Положительный вес вложения в граммах.
    /// @param basePrice Неотрицательная базовая плата в копейках.
    /// @param pricePerKilogram Положительный тариф за начатый килограмм в копейках.
    /// @throws std::invalid_argument Вес, тариф или параметры услуги недопустимы.
    /// @throws std::overflow_error Итоговая цена не помещается в int.
    Parcel(const std::string &trackingNumber, const Customer &sender,
           const Customer &recipient, int weightGrams, int basePrice,
           int pricePerKilogram);

    /// @brief Масса содержимого в граммах.
    /// @return Масса содержимого в граммах.
    int weightGrams() const override;
    /// @brief Базовая плата в копейках.
    /// @return Базовая плата в копейках.
    int basePrice() const;
    /// @brief Тариф в копейках за начатый килограмм.
    /// @return Тариф в копейках за начатый килограмм.
    int pricePerKilogram() const;
    /// @brief Округляет положительный вес вверх до числа оплачиваемых килограммов.
    /// @return Число оплачиваемых килограммов с округлением вверх.
    int chargedKilograms() const;
    /// @brief Рассчитывает стоимость пересылки в копейках.
    /// @return Стоимость пересылки в копейках.
    int calculatePrice() const override;
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    std::unique_ptr<Shipment> clone() const override;
};

#endif
