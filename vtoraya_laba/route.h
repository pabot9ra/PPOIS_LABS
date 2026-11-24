/// @file route.h
/// @author Владислав Вертинский
/// @brief Упорядоченный план остановок транспорта.

#ifndef ROUTE_H
#define ROUTE_H

#include "routestop.h"
#include <cstddef>
#include <string>
#include <vector>

/// @brief Упорядоченный план остановок транспорта.
/// @details Первая входящая дистанция равна нулю, время остановок строго возрастает, соседние отделения различаются.
class Route {
private:
    int id_; ///< Идентификатор объекта.
    std::string name_; ///< Имя или название объекта.
    std::vector<RouteStop> stops_; ///< Остановки в порядке следования.

public:
    /// @brief Создаёт маршрут без остановок.
    /// @param id Идентификатор записи.
    /// @param name Непустое имя или название.
    Route(int id, const std::string &name);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Название маршрута.
    /// @return Название маршрута.
    const std::string &name() const;
    /// @brief Число остановок маршрута.
    /// @return Число остановок маршрута.
    std::size_t stopCount() const;
    /// @brief Находит остановку по индексу от нуля.
    /// @param index Индекс элемента, начиная с нуля.
    /// @return Ссылка на остановку с указанным индексом.
    const RouteStop &stopAt(std::size_t index) const;
    /// @brief Добавляет остановку с проверкой порядка времени, отделения и суммарной дистанции.
    /// @param stop Добавляемая остановка.
    void addStop(const RouteStop &stop);
    /// @brief Суммирует входящие расстояния остановок в километрах.
    /// @return Общая дистанция маршрута в километрах.
    int totalDistanceKm() const;
    /// @brief Проверяет наличие отделения среди остановок строго после текущей.
    /// @param currentStop Индекс текущей остановки, начиная с нуля.
    /// @param branchId Идентификатор отделения.
    /// @return true, если отделение есть среди следующих остановок; иначе false.
    bool canReachAfter(std::size_t currentStop, int branchId) const;
};

#endif
