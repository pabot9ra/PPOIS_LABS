/// @file workinghours.h
/// @author Владислав Вертинский
/// @brief Расписание одного дня с одним перерывом.

#ifndef WORKINGHOURS_H
#define WORKINGHOURS_H

/// @brief Расписание одного дня с одним перерывом.
/// @details Время в минутах от начала суток. Интервалы полуоткрытые; равные границы перерыва означают его отсутствие.
class WorkingHours {
private:
    int opensAt_; ///< Минута открытия.
    int closesAt_; ///< Минута закрытия, не включая её.
    int breakStart_; ///< Первая минута перерыва.
    int breakEnd_; ///< Первая минута после перерыва.

    /// @brief Проверяет диапазон минуты от начала суток.
    /// @param minute Минута от начала суток, от 0 до 1439.
    static void validateMinute(int minute);

public:
    static const int MINUTES_PER_DAY = 24 * 60; ///< Число минут в сутках.

    /// @brief Создаёт расписание с ненулевым рабочим временем.
    /// @param opensAt Минута открытия от начала суток.
    /// @param closesAt Минута закрытия; может быть равна 1440.
    /// @param breakStart Первая минута перерыва внутри рабочего интервала.
    /// @param breakEnd Первая минута после перерыва; равенство breakStart означает отсутствие перерыва.
    /// @throws std::invalid_argument Интервалы некорректны или нет рабочего времени.
    WorkingHours(int opensAt, int closesAt, int breakStart, int breakEnd);

    /// @brief Минута открытия от начала суток.
    /// @return Минута открытия от начала суток.
    int opensAt() const;
    /// @brief Минута закрытия, не включая эту минуту.
    /// @return Минута закрытия, не включая эту минуту.
    int closesAt() const;
    /// @brief Первая минута перерыва.
    /// @return Первая минута перерыва.
    int breakStart() const;
    /// @brief Первая минута после перерыва.
    /// @return Первая минута после перерыва.
    int breakEnd() const;
    /// @brief Проверяет, принадлежит ли минута рабочему интервалу вне перерыва.
    /// @param minute Минута от начала суток, от 0 до 1439.
    /// @return true для рабочей минуты вне перерыва; иначе false.
    bool isOpenAt(int minute) const;
    /// @brief Вычисляет длительность рабочего времени без перерыва.
    /// @return Число рабочих минут без перерыва.
    int workingMinutes() const;
    /// @brief Находит ближайшую рабочую минуту не раньше заданной в пределах этого дня.
    /// @param minute Минута от начала суток, от 0 до 1439.
    /// @return Ближайшая рабочая минута этого дня; если её нет, выбрасывается исключение.
    int nextOpenMinute(int minute) const;
    /// @brief Проверяет, помещается ли обслуживание целиком в рабочий интервал без перерыва.
    /// @param startMinute Минута начала обслуживания от начала суток.
    /// @param durationMinutes Положительная длительность обслуживания.
    /// @return true, если обслуживание целиком укладывается в рабочий интервал; иначе false.
    bool canServe(int startMinute, int durationMinutes) const;
};

#endif
