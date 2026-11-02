/// @file face.h
/// @author Владислав Вертинский
/// @brief Грань кубика Рубика и цвета наклеек

#ifndef FACE_H
#define FACE_H
#include <array>
#include <iosfwd>

/// @brief Цвет наклейки
/// @details в файле и на экране каждый цвет записывается одной буквой
enum class Color{
    Blue,    ///< B - синий
    Red,     ///< R - красный
    Yellow,  ///< Y - жёлтый
    Green,   ///< G - зелёный
    White,   ///< W - белый
    Orange   ///< O - оранжевый
};

/// @brief Одна грань кубика - матрица 3 на 3 цветов
/// @details клетка задаётся рядом и столбцом от 0 до 2, ряд 0 - верхний, столбец 0 - левый, если смотреть на грань прямо.
/// Грань ничего не знает о соседних гранях: перенос наклеек между гранями при ходе делает Cube
class Face{
    private:
        std::array<std::array<Color, 3>, 3> cells_;
    public:
        /// @brief создание грани, все 9 клеток которой одного цвета
        /// @details explicit запрещает неявно превращать Color в Face
        /// @param color цвет всех клеток
        explicit Face(Color color);

        /// @brief возвращает цвет клетки
        /// @details номера не проверяются: значение вне 0..2 - неопределённое поведение
        /// @param r ряд от 0 до 2
        /// @param c столбец от 0 до 2
        /// @return цвет клетки
        Color at(int r, int c) const;

        /// @brief задаёт цвет клетки
        /// @details номера не проверяются: значение вне 0..2 - неопределённое поведение
        /// @param r ряд от 0 до 2
        /// @param c столбец от 0 до 2
        /// @param color новый цвет клетки
        void set(int r, int c, Color color);

        /// @brief поворачивает грань на 90 градусов по часовой стрелке
        /// @details клетка из [r][c] переходит в [c][2 - r], центр остаётся на месте. Соседние грани не меняются
        void rotateClockwise();

        /// @brief проверяет, что грань одноцветная
        /// @return true если все 9 клеток одного цвета
        bool isSolved() const;

        /// @brief позволяет проверить на равность две грани
        /// @details грани равны, если совпадают цвета во всех 9 клетках
        /// @param other грань для сравнения
        /// @return true если грани равны, false когда неравны
        bool operator==(const Face &other) const;

        /// @brief позволяет проверить на неравность две грани
        /// @param other грань для сравнения
        /// @return true если грани неравны, false когда равны
        bool operator!=(const Face &other) const;
};

/// @brief позволяет выводить цвет в поток
/// @details выводится одна буква: W, Y, R, O, G или B
/// @param os поток вывода
/// @param color цвет
/// @return возвращает поток вывода
std::ostream& operator<<(std::ostream &os, Color color);

/// @brief позволяет читать цвет из потока
/// @details читается одна буква, пробелы перед ней пропускаются. При неизвестной букве поток переводится в состояние ошибки, а color не меняется
/// @param is поток ввода
/// @param color цвет, в который записывается результат
/// @return возвращает поток ввода
std::istream& operator>>(std::istream &is, Color &color);

#endif
