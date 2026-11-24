/// @file packaging.h
/// @author Владислав Вертинский
/// @brief Абстрактная упаковка с ограничениями веса и размеров.

#ifndef PACKAGING_H
#define PACKAGING_H

#include "dimensions.h"
#include "shipment.h"
#include <string>

/// @brief Абстрактная упаковка с ограничениями веса и размеров.
/// @details Хранит трек-номер вложения. Упаковка и отправление остаются отдельными объектами.
class Packaging {
private:
    Dimensions innerDimensions_; ///< Полезные внутренние размеры.
    int maximumWeight_; ///< Предельный вес вложений, г.
    int price_; ///< Цена упаковки, коп.
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    bool sealed_; ///< Признак пломбы или запечатывания.

public:
    /// @brief Создаёт пустую незапечатанную упаковку.
    /// @param innerDimensions Полезные габариты упаковки в миллиметрах.
    /// @param maximumWeight Положительный предел веса вложений в граммах.
    /// @param price Неотрицательная цена упаковки в копейках.
    Packaging(const Dimensions &innerDimensions, int maximumWeight, int price);
    /// @brief Освобождает объект при удалении через базовый указатель.
    virtual ~Packaging() = default;

    /// @brief Полезные внутренние размеры.
    /// @return Полезные внутренние размеры.
    const Dimensions &innerDimensions() const;
    /// @brief Предельная масса вложений в граммах.
    /// @return Предельная масса вложений в граммах.
    int maximumWeight() const;
    /// @brief Стоимость упаковки в копейках.
    /// @return Стоимость упаковки в копейках.
    int price() const;
    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Признак запечатанного состояния.
    /// @return Признак запечатанного состояния.
    bool sealed() const;
    /// @brief Формирует маркировку конкретного вида упаковки.
    /// @return Маркировка вида упаковки.
    virtual std::string label() const = 0;

    /// @brief Размещает принятое отправление в пустой упаковке с проверкой веса и габаритов.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param contents Габариты вложения; при подборе допускается перестановка осей.
    void pack(const Shipment &shipment, const Dimensions &contents);
    /// @brief Запечатывает заполненный открытый объект.
    void seal();
    /// @brief Открывает запечатанный объект.
    void open();
    /// @brief Очищает открытую заполненную упаковку.
    void unpack();
};

#endif
