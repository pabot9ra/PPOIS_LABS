/// @file box.h
/// @author Владислав Вертинский
/// @brief Коробка с защитным наполнителем.

#ifndef BOX_H
#define BOX_H

#include "packaging.h"
#include <string>

/// @brief Коробка с защитным наполнителем.
/// @details Полезные размеры меньше внешних на двойную толщину наполнителя по каждой оси.
class Box : public Packaging {
private:
    int padding_; ///< Наполнитель с каждой стороны, мм.
    std::string material_; ///< Материал коробки.

public:
    /// @brief Создаёт пустую коробку с учётом наполнителя.
    /// @param outerDimensions Внешние габариты коробки в миллиметрах.
    /// @param maximumWeight Положительный предел веса вложений в граммах.
    /// @param price Неотрицательная цена упаковки в копейках.
    /// @param padding Неотрицательная толщина наполнителя с каждой стороны, мм.
    /// @param material Непустое название материала коробки.
    Box(const Dimensions &outerDimensions, int maximumWeight, int price,
        int padding, const std::string &material);

    /// @brief Толщина наполнителя с каждой стороны в миллиметрах.
    /// @return Толщина наполнителя с каждой стороны в миллиметрах.
    int padding() const;
    /// @brief Материал коробки.
    /// @return Материал коробки.
    const std::string &material() const;
    /// @brief Восстанавливает внешние размеры коробки по полезным размерам и наполнителю.
    /// @return Новые размеры с учётом наполнителя с обеих сторон.
    Dimensions outerDimensions() const;
    /// @brief Формирует маркировку конкретного вида упаковки.
    /// @return Маркировка вида упаковки.
    std::string label() const override;
};

#endif
