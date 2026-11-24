/// @file fragileparcel.h
/// @author Владислав Вертинский
/// @brief Хрупкая посылка с маркировкой и требованием ориентации.

#ifndef FRAGILEPARCEL_H
#define FRAGILEPARCEL_H

#include "parcel.h"

/// @brief Хрупкая посылка с маркировкой и требованием ориентации.
/// @details Доплата прибавляется к цене посылки. Проверка ориентации вызывается явно.
class FragileParcel : public Parcel {
private:
    int packagingFee_; ///< Плата за упаковку, коп.
    bool keepUpright_; ///< Требование вертикальной ориентации.

public:
    /// @brief Создаёт хрупкую посылку с правилами обращения.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param sender Отправитель; сохраняется копией.
    /// @param recipient Получатель; сохраняется копией.
    /// @param weightGrams Положительный вес вложения в граммах.
    /// @param basePrice Неотрицательная базовая плата в копейках.
    /// @param pricePerKilogram Положительный тариф за начатый килограмм в копейках.
    /// @param packagingFee Неотрицательная доплата за упаковку в копейках.
    /// @param keepUpright Требуется ли вертикальное положение.
    /// @throws std::invalid_argument Вес, тариф или параметры услуги недопустимы.
    /// @throws std::overflow_error Итоговая цена не помещается в int.
    FragileParcel(const std::string &trackingNumber, const Customer &sender,
                  const Customer &recipient, int weightGrams, int basePrice,
                  int pricePerKilogram, int packagingFee, bool keepUpright);

    /// @brief Доплата за упаковку в копейках.
    /// @return Доплата за упаковку в копейках.
    int packagingFee() const;
    /// @brief Требование сохранять вертикальное положение.
    /// @return Требование сохранять вертикальное положение.
    bool keepUpright() const;
    /// @brief Формирует маркировку хрупкого груза с учётом требования ориентации.
    /// @return Маркировка правил обращения с посылкой.
    std::string handlingLabel() const;
    /// @brief Отклоняет неверное положение посылки, если требуется вертикальная ориентация.
    /// @param upright Вертикально ли фактически расположено вложение.
    void checkOrientation(bool upright) const;
    /// @brief Рассчитывает стоимость пересылки в копейках.
    /// @return Стоимость пересылки в копейках.
    int calculatePrice() const override;
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    std::unique_ptr<Shipment> clone() const override;
};

#endif
