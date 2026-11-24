/// @file shipment.h
/// @author Владислав Вертинский
/// @brief Абстрактное почтовое отправление.

#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "customer.h"
#include <memory>
#include <string>

/// @brief Этап доставки отправления.
enum class ShipmentStatus {
    Accepted, ///< Принято к отправке.
    InTransit, ///< В пути.
    Delivered ///< Доставлено.
};

/// @brief Абстрактное почтовое отправление.
/// @details Хранит копии участников. Допустимы переходы Accepted → InTransit → Delivered. Цена и клонирование зависят от вида отправления.
class Shipment {
private:
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    Customer sender_; ///< Снимок данных отправителя.
    Customer recipient_; ///< Снимок данных получателя.
    ShipmentStatus status_; ///< Текущее состояние объекта.

public:
    /// @brief Создаёт принятое отправление и проверяет трек-номер.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param sender Отправитель; сохраняется копией.
    /// @param recipient Получатель; сохраняется копией.
    /// @throws std::invalid_argument Трек-номер пустой или содержит пробельные символы.
    Shipment(const std::string &trackingNumber, const Customer &sender,
             const Customer &recipient);
    /// @brief Освобождает объект при удалении через базовый указатель.
    virtual ~Shipment() = default;

    /// @brief Рассчитывает стоимость пересылки в копейках.
    /// @return Стоимость пересылки в копейках.
    virtual int calculatePrice() const = 0;
    /// @brief Масса содержимого в граммах.
    /// @return Масса содержимого в граммах.
    virtual int weightGrams() const = 0;
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    virtual std::unique_ptr<Shipment> clone() const = 0;

    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Сохранённая копия отправителя.
    /// @return Сохранённая копия отправителя.
    const Customer &sender() const;
    /// @brief Сохранённая копия получателя.
    /// @return Сохранённая копия получателя.
    const Customer &recipient() const;
    /// @brief Текущее состояние объекта.
    /// @return Текущее состояние объекта.
    ShipmentStatus status() const;

    /// @brief Переводит принятое отправление в состояние «в пути».
    /// @throws ShipmentStateException Отправление не находится в Accepted.
    void dispatch();
    /// @brief Отмечает доставку находящегося в пути отправления.
    /// @throws ShipmentStateException Отправление не находится в InTransit.
    void deliver();
};

#endif
