/// @file storagecell.h
/// @author Владислав Вертинский
/// @brief Одиночная ячейка хранения с ограничением веса и размеров.

#ifndef STORAGECELL_H
#define STORAGECELL_H

#include "dimensions.h"
#include "shipment.h"
#include <string>

/// @brief Одиночная ячейка хранения с ограничением веса и размеров.
/// @details Хранит трек-номер и вес, а не сам объект отправления. При размещении учитывается перестановка осей габаритов.
class StorageCell {
private:
    std::string id_; ///< Идентификатор объекта.
    int branchId_; ///< Идентификатор отделения.
    Dimensions dimensions_; ///< Габариты ячейки, мм.
    int maximumWeight_; ///< Предельный вес вложений, г.
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    int weightGrams_; ///< Вес содержимого или записи, г.

public:
    /// @brief Создаёт пустую ячейку хранения.
    /// @param id Идентификатор записи.
    /// @param branchId Идентификатор отделения.
    /// @param dimensions Габариты ячейки в миллиметрах.
    /// @param maximumWeight Положительный предел веса вложений в граммах.
    StorageCell(const std::string &id, int branchId, const Dimensions &dimensions, int maximumWeight);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    const std::string &id() const;
    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Габариты ячейки в миллиметрах.
    /// @return Габариты ячейки в миллиметрах.
    const Dimensions &dimensions() const;
    /// @brief Предельная масса вложений в граммах.
    /// @return Предельная масса вложений в граммах.
    int maximumWeight() const;
    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Масса содержимого в граммах.
    /// @return Масса содержимого в граммах.
    int weightGrams() const;
    /// @brief Признак занятой ячейки.
    /// @return Признак занятой ячейки.
    bool occupied() const;
    /// @brief Проверяет свободное место, состояние отправления, вес и габариты.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param contents Габариты вложения; при подборе допускается перестановка осей.
    /// @return true, если свободная ячейка допускает размещение отправления; иначе false.
    bool canFit(const Shipment &shipment, const Dimensions &contents) const;
    /// @brief Сохраняет номер и вес отправления в свободной ячейке.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param contents Габариты вложения; при подборе допускается перестановка осей.
    void store(const Shipment &shipment, const Dimensions &contents);
    /// @brief Освобождает занятую ячейку и обнуляет её вес.
    void release();
};

#endif
