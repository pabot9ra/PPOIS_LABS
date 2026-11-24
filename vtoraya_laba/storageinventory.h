/// @file storageinventory.h
/// @author Владислав Вертинский
/// @brief Складской учёт одного отделения с общим пределом веса.

#ifndef STORAGEINVENTORY_H
#define STORAGEINVENTORY_H

#include "storagezone.h"
#include "storagelocation.h"
#include "postalbranch.h"
#include <cstddef>
#include <vector>

/// @brief Складской учёт одного отделения с общим пределом веса.
/// @details Владеет копиями пустых зон; один трек-номер размещается не более чем в одной ячейке.
class StorageInventory {
private:
    int branchId_; ///< Идентификатор отделения.
    int maximumWeight_; ///< Предельный вес вложений, г.
    std::vector<StorageZone> zones_; ///< Копии зон хранения.

    /// @brief Проверяет состояние отправления, уникальность трек-номера и общую грузоподъёмность склада.
    /// @param shipment Отправление, над которым выполняется операция.
    void ensureCanStore(const Shipment &shipment) const;

public:
    /// @brief Создаёт пустой складской учёт.
    /// @param branch Отделение, с которым связана операция.
    /// @param maximumWeight Положительный предел веса вложений в граммах.
    StorageInventory(const PostalBranch &branch, int maximumWeight);

    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Число зарегистрированных зон.
    /// @return Число зарегистрированных зон.
    std::size_t zoneCount() const;
    /// @brief Число учтённых отправлений.
    /// @return Число учтённых отправлений.
    std::size_t shipmentCount() const;
    /// @brief Масса содержимого в граммах.
    /// @return Масса содержимого в граммах.
    long long weightGrams() const;
    /// @brief Вычисляет оставшуюся грузоподъёмность в граммах.
    /// @return Допустимый вес дополнительных вложений в граммах.
    long long remainingCapacity() const;
    /// @brief Добавляет копию пустой зоны своего отделения.
    /// @param zone Пустая зона нужного отделения.
    void addZone(const StorageZone &zone);
    /// @brief Проверяет наличие записи по идентификатору.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return true, если запись найдена; иначе false.
    bool contains(const std::string &trackingNumber) const;
    /// @brief Размещает отправление в первой подходящей свободной ячейке.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param contents Габариты вложения; при подборе допускается перестановка осей.
    /// @return Адрес занятой ячейки.
    StorageLocation storeShipment(const Shipment &shipment, const Dimensions &contents);
    /// @brief Находит самостоятельный адрес занятой ячейки.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return Место учёта или хранения найденного отправления.
    StorageLocation locateShipment(const std::string &trackingNumber) const;
    /// @brief Получает зафиксированный вес хранящегося отправления.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return Сохранённый вес отправления в граммах.
    int storedWeightGrams(const std::string &trackingNumber) const;
    /// @brief Удаляет запись отправления из хранилища, освобождая ячейку.
    /// @param trackingNumber Трек-номер без пробельных символов.
    void releaseShipment(const std::string &trackingNumber);
};

#endif
