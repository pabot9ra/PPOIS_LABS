/// @file dimensions.h
/// @author Владислав Вертинский
/// @brief Габариты прямоугольного содержимого в миллиметрах.

#ifndef DIMENSIONS_H
#define DIMENSIONS_H

/// @brief Габариты прямоугольного содержимого в миллиметрах.
/// @details Все стороны положительны; объём должен помещаться в long long. Подбор проверяет шесть перестановок осей.
class Dimensions {
private:
    int length_; ///< Длина, мм.
    int width_; ///< Ширина, мм.
    int height_; ///< Высота, мм.

public:
    /// @brief Создаёт размеры и проверяет допустимость сторон и объёма.
    /// @param length Положительная длина в миллиметрах.
    /// @param width Положительная ширина в миллиметрах.
    /// @param height Положительная высота в миллиметрах.
    Dimensions(int length, int width, int height);

    /// @brief Длина в миллиметрах.
    /// @return Длина в миллиметрах.
    int length() const;
    /// @brief Ширина в миллиметрах.
    /// @return Ширина в миллиметрах.
    int width() const;
    /// @brief Высота в миллиметрах.
    /// @return Высота в миллиметрах.
    int height() const;
    /// @brief Вычисляет объём в кубических миллиметрах.
    /// @return Объём в кубических миллиметрах.
    long long volume() const;
    /// @brief Проверяет размещение внутри других размеров с учётом шести перестановок осей.
    /// @param other Другой объект для сравнения или копирования.
    /// @return true, если размещение возможно хотя бы при одной перестановке осей; иначе false.
    bool fitsInside(const Dimensions &other) const;
    /// @brief Вычитает толщину наполнителя с обеих сторон по каждой оси.
    /// @param padding Неотрицательная толщина наполнителя с каждой стороны, мм.
    /// @return Новые уменьшенные размеры без изменения исходных.
    Dimensions inset(int padding) const;
};

#endif
