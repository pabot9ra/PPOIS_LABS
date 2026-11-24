/// @file envelope.h
/// @author Владислав Вертинский
/// @brief Конверт с признаками усиления и влагозащиты.

#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "packaging.h"

/// @brief Конверт с признаками усиления и влагозащиты.
/// @details Ограничения веса и габаритов, заполнение и запечатывание реализованы в Packaging.
class Envelope : public Packaging {
private:
    bool reinforced_; ///< Признак усиления конверта.
    bool waterproof_; ///< Признак влагозащиты.

public:
    /// @brief Создаёт пустой конверт с заданными защитными свойствами.
    /// @param innerDimensions Полезные габариты упаковки в миллиметрах.
    /// @param maximumWeight Положительный предел веса вложений в граммах.
    /// @param price Неотрицательная цена упаковки в копейках.
    /// @param reinforced Признак усиления конверта.
    /// @param waterproof Признак влагозащиты конверта.
    Envelope(const Dimensions &innerDimensions, int maximumWeight, int price,
             bool reinforced, bool waterproof);

    /// @brief Признак усиленного конверта.
    /// @return Признак усиленного конверта.
    bool reinforced() const;
    /// @brief Признак влагостойкого конверта.
    /// @return Признак влагостойкого конверта.
    bool waterproof() const;
    /// @brief Формирует маркировку конкретного вида упаковки.
    /// @return Маркировка вида упаковки.
    std::string label() const override;
};

#endif
