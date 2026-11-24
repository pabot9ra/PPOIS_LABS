/// @file trackingevent.h
/// @author Владислав Вертинский
/// @brief Снимок статуса отправления в отделении в заданную минуту.

#ifndef TRACKINGEVENT_H
#define TRACKINGEVENT_H

#include "postalbranch.h"
#include <string>

/// @brief Снимок статуса отправления в отделении в заданную минуту.
/// @details Статус считывается из записи отделения при создании; последующие изменения отправления не меняют событие.
class TrackingEvent {
private:
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    ShipmentStatus status_; ///< Текущее состояние объекта.
    int branchId_; ///< Идентификатор отделения.
    int minute_; ///< Минута события.
    std::string description_; ///< Описание события.

public:
    /// @brief Фиксирует событие для существующего в отделении отправления.
    /// @param branch Отделение, с которым связана операция.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param minute Неотрицательная минута от начала отслеживания.
    /// @param description Непустое описание события.
    /// @note Время задаётся вызывающим кодом и не считывается с системных часов.
    TrackingEvent(const PostalBranch &branch, const std::string &trackingNumber,
                  int minute, const std::string &description);

    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Текущее состояние объекта.
    /// @return Текущее состояние объекта.
    ShipmentStatus status() const;
    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Время события в минутах от начала отслеживания.
    /// @return Время события в минутах от начала отслеживания.
    int minute() const;
    /// @brief Описание события.
    /// @return Описание события.
    const std::string &description() const;
    /// @brief Формирует строку с трек-номером, отделением, минутой и описанием.
    /// @return Текстовое представление записи.
    std::string format() const;
};

#endif
