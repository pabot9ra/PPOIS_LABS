/// @file postalbranch.h
/// @author Владислав Вертинский
/// @brief Отделение, владеющее полиморфными копиями отправлений.

#ifndef POSTALBRANCH_H
#define POSTALBRANCH_H

#include "shipment.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

/// @brief Отделение, владеющее полиморфными копиями отправлений.
/// @details Приём клонирует конкретный вид отправления. Изменение внешнего объекта не меняет сохранённую запись.
class PostalBranch {
private:
    int id_; ///< Идентификатор объекта.
    PostalAddress address_; ///< Собственная копия адреса.
    std::vector<std::unique_ptr<Shipment>> shipments_; ///< Полиморфные копии отправлений отделения.

    /// @brief Находит отправление для внутреннего изменения.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return Находит отправление для внутреннего изменения.
    Shipment &findMutableShipment(const std::string &trackingNumber);

public:
    /// @brief Создаёт отделение без отправлений.
    /// @param id Идентификатор записи.
    /// @param address Почтовый адрес, сохраняемый копией.
    PostalBranch(int id, const PostalAddress &address);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Адрес объекта.
    /// @return Адрес объекта.
    const PostalAddress &address() const;
    /// @brief Число учтённых отправлений.
    /// @return Число учтённых отправлений.
    std::size_t shipmentCount() const;
    /// @brief Проверяет наличие трек-номера среди сохранённых отправлений.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return true, если отправление зарегистрировано; иначе false.
    bool containsShipment(const std::string &trackingNumber) const;

    /// @brief Клонирует принятое отправление, исключая повтор трек-номера внутри отделения.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @throws ShipmentStateException Отправление уже отправлено или доставлено.
    /// @throws DuplicateShipmentException Трек-номер уже зарегистрирован.
    void acceptShipment(const Shipment &shipment);
    /// @brief Находит сохранённое отправление по трек-номеру.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return Ссылка на сохранённое отправление.
    /// @note Ссылка сохраняется до уничтожения отделения; внешняя копия отправления не используется.
    /// @throws ShipmentNotFoundException Отправление не найдено в отделении.
    const Shipment &findShipment(const std::string &trackingNumber) const;
    /// @brief Переводит сохранённое отправление в состояние «в пути».
    /// @param trackingNumber Трек-номер без пробельных символов.
    void dispatchShipment(const std::string &trackingNumber);
    /// @brief Отмечает доставку сохранённого отправления.
    /// @param trackingNumber Трек-номер без пробельных символов.
    void deliverShipment(const std::string &trackingNumber);
    /// @brief Фиксирует однократное подтверждение получения заказного письма после доставки.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param signer Непустое имя подтвердившего получение.
    void confirmReceipt(const std::string &trackingNumber, const std::string &signer);
    /// @brief Учитывает компенсацию после доставки в пределах остатка страхового покрытия.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param amount Сумма операции в копейках.
    void recordCompensation(const std::string &trackingNumber, int amount);
};

#endif
