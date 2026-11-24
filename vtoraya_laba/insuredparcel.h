/// @file insuredparcel.h
/// @author Владислав Вертинский
/// @brief Посылка с объявленной ценностью и учётом компенсаций.

#ifndef INSUREDPARCEL_H
#define INSUREDPARCEL_H

#include "parcel.h"

/// @brief Посылка с объявленной ценностью и учётом компенсаций.
/// @details Сумма компенсаций ограничена объявленной ценностью. Метод учёта не осуществляет денежный перевод.
class InsuredParcel : public Parcel {
private:
    int declaredValue_; ///< Объявленная ценность, коп.
    int insuranceFee_; ///< Страховая плата, коп.
    int compensationRecorded_; ///< Уже учтённая компенсация, коп.

public:
    /// @brief Создаёт застрахованную посылку без учтённых компенсаций.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param sender Отправитель; сохраняется копией.
    /// @param recipient Получатель; сохраняется копией.
    /// @param weightGrams Положительный вес вложения в граммах.
    /// @param basePrice Неотрицательная базовая плата в копейках.
    /// @param pricePerKilogram Положительный тариф за начатый килограмм в копейках.
    /// @param declaredValue Положительная объявленная ценность в копейках.
    /// @param insuranceFee Неотрицательная страховая плата в копейках.
    /// @throws std::invalid_argument Вес, тариф или параметры услуги недопустимы.
    /// @throws std::overflow_error Итоговая цена не помещается в int.
    InsuredParcel(const std::string &trackingNumber, const Customer &sender,
                  const Customer &recipient, int weightGrams, int basePrice,
                  int pricePerKilogram, int declaredValue, int insuranceFee);

    /// @brief Объявленная ценность в копейках.
    /// @return Объявленная ценность в копейках.
    int declaredValue() const;
    /// @brief Страховая плата в копейках.
    /// @return Страховая плата в копейках.
    int insuranceFee() const;
    /// @brief Вычисляет остаток страхового покрытия в копейках.
    /// @return Остаток страхового покрытия в копейках.
    int remainingCoverage() const;
    /// @brief Учитывает компенсацию после доставки в пределах остатка страхового покрытия.
    /// @param amount Сумма операции в копейках.
    void recordCompensation(int amount);
    /// @brief Рассчитывает стоимость пересылки в копейках.
    /// @return Стоимость пересылки в копейках.
    int calculatePrice() const override;
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    std::unique_ptr<Shipment> clone() const override;
};

#endif
