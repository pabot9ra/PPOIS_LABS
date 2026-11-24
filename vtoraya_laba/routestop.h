/// @file routestop.h
/// @author Владислав Вертинский
/// @brief Отделение, входящая дистанция и плановое время остановки.

#ifndef ROUTESTOP_H
#define ROUTESTOP_H

#include "postalbranch.h"
#include <string>

/// @brief Отделение, входящая дистанция и плановое время остановки.
/// @details Адрес отделения сохраняется копией; время отсчитывается в минутах от начала маршрута.
class RouteStop {
private:
    int branchId_; ///< Идентификатор отделения.
    PostalAddress address_; ///< Собственная копия адреса.
    int distanceKm_; ///< Входящее расстояние, км.
    int plannedMinute_; ///< Плановая минута маршрута.

public:
    /// @brief Создаёт остановку маршрута.
    /// @param branch Отделение, с которым связана операция.
    /// @param distanceKm Неотрицательное расстояние от предыдущей остановки в километрах.
    /// @param plannedMinute Плановая минута от начала маршрута.
    RouteStop(const PostalBranch &branch, int distanceKm, int plannedMinute);

    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Адрес объекта.
    /// @return Адрес объекта.
    const PostalAddress &address() const;
    /// @brief Расстояние от предыдущей остановки в километрах.
    /// @return Расстояние от предыдущей остановки в километрах.
    int distanceKm() const;
    /// @brief Плановая минута от начала маршрута.
    /// @return Плановая минута от начала маршрута.
    int plannedMinute() const;
    /// @brief Формирует описание остановки с адресом и плановым временем.
    /// @return Описание остановки маршрута.
    std::string describe() const;
};

#endif
