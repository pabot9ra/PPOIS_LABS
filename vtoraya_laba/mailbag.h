/// @file mailbag.h
/// @author Владислав Вертинский
/// @brief Почтовый мешок для перевозки между двумя отделениями.

#ifndef MAILBAG_H
#define MAILBAG_H

#include "shipment.h"
#include <cstddef>
#include <string>
#include <vector>

/// @brief Почтовый мешок для перевозки между двумя отделениями.
/// @details Хранит трек-номера и суммарный вес вложений, не владеет отправлениями. Масса тары не учитывается.
class MailBag {
private:
    std::string id_; ///< Идентификатор объекта.
    int sourceBranchId_; ///< Отделение отправления мешка.
    int destinationBranchId_; ///< Отделение назначения мешка.
    int maximumWeight_; ///< Предельный вес вложений, г.
    int weightGrams_; ///< Вес содержимого или записи, г.
    std::vector<std::string> trackingNumbers_; ///< Снимок списка трек-номеров мешка.
    bool sealed_; ///< Признак пломбы или запечатывания.

public:
    /// @brief Создаёт пустой незапечатанный мешок.
    /// @param id Идентификатор записи.
    /// @param sourceBranchId Положительный ID отделения отправления мешка.
    /// @param destinationBranchId Положительный ID другого отделения назначения.
    /// @param maximumWeight Положительный предел веса вложений в граммах.
    MailBag(const std::string &id, int sourceBranchId, int destinationBranchId, int maximumWeight);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    const std::string &id() const;
    /// @brief Идентификатор отделения отправления мешка.
    /// @return Идентификатор отделения отправления мешка.
    int sourceBranchId() const;
    /// @brief Идентификатор отделения назначения мешка.
    /// @return Идентификатор отделения назначения мешка.
    int destinationBranchId() const;
    /// @brief Предельная масса вложений в граммах.
    /// @return Предельная масса вложений в граммах.
    int maximumWeight() const;
    /// @brief Масса содержимого в граммах.
    /// @return Масса содержимого в граммах.
    int weightGrams() const;
    /// @brief Число учтённых отправлений.
    /// @return Число учтённых отправлений.
    std::size_t shipmentCount() const;
    /// @brief Возвращает трек-номер по индексу.
    /// @param index Индекс элемента, начиная с нуля.
    /// @return Трек-номер вложения с указанным индексом.
    const std::string &trackingNumberAt(std::size_t index) const;
    /// @brief Признак запечатанного состояния.
    /// @return Признак запечатанного состояния.
    bool sealed() const;
    /// @brief Вычисляет оставшуюся грузоподъёмность в граммах.
    /// @return Допустимый вес дополнительных вложений в граммах.
    int remainingCapacity() const;
    /// @brief Проверяет наличие записи по идентификатору.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return true, если запись найдена; иначе false.
    bool contains(const std::string &trackingNumber) const;
    /// @brief Добавляет трек-номер и вес недоставленного отправления в открытый мешок.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @throws CapacityExceededException Вес превышает остаток вместимости.
    /// @throws std::logic_error Мешок запечатан или отправление доставлено.
    /// @throws std::invalid_argument Трек-номер уже присутствует в мешке.
    void addShipment(const Shipment &shipment);
    /// @brief Запечатывает непустой открытый мешок.
    void seal();
    /// @brief Снимает пломбу мешка, сохраняя сведения о вложениях.
    void open();
    /// @brief Удаляет записи вложений из открытого мешка и обнуляет вес.
    void clear();
};

#endif
