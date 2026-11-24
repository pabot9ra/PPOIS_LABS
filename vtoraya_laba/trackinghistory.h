/// @file trackinghistory.h
/// @author Владислав Вертинский
/// @brief Явно пополняемая хронологическая история отправления.

#ifndef TRACKINGHISTORY_H
#define TRACKINGHISTORY_H

#include "trackingevent.h"
#include <cstddef>
#include <vector>

/// @brief Явно пополняемая хронологическая история отправления.
/// @details Первое событие — приём; нельзя пропускать отправку или возвращаться к предыдущему статусу. Одинаковое время событий допустимо.
class TrackingHistory {
private:
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    std::vector<TrackingEvent> events_; ///< Снимки событий в порядке записи.

    /// @brief Проверяет допустимость следующего статуса в истории.
    /// @param next Следующий проверяемый статус.
    void validateTransition(ShipmentStatus next) const;

public:
    /// @brief Создаёт пустую историю указанного трек-номера.
    /// @param shipment Отправление, над которым выполняется операция.
    explicit TrackingHistory(const Shipment &shipment);

    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Число записанных событий.
    /// @return Число записанных событий.
    std::size_t eventCount() const;
    /// @brief Находит событие по индексу от нуля.
    /// @param index Индекс элемента, начиная с нуля.
    /// @return Ссылка на событие с указанным индексом.
    /// @note Добавление нового события может сделать ранее полученную ссылку недействительной.
    const TrackingEvent &eventAt(std::size_t index) const;
    /// @brief Последнее записанное событие.
    /// @return Последнее записанное событие.
    /// @note Добавление нового события может сделать ранее полученную ссылку недействительной.
    const TrackingEvent &lastEvent() const;
    /// @brief Добавляет событие после проверки трек-номера, времени и перехода статуса.
    /// @param event Снимок события нужного отправления.
    /// @throws std::invalid_argument Неверен трек-номер или нарушена хронология.
    /// @throws std::logic_error Нарушена последовательность статусов.
    void record(const TrackingEvent &event);
    /// @brief Разность времени последнего и первого события; ноль для пустой истории.
    /// @return Разность времени последнего и первого события; ноль для пустой истории.
    int elapsedMinutes() const;
    /// @brief Проверяет наличие хотя бы одного события с указанным статусом.
    /// @param status Проверяемый статус отправления.
    /// @return true, если такой статус встречается в истории; иначе false.
    bool hasStatus(ShipmentStatus status) const;
    /// @brief Находит последнее по порядку событие указанного отделения.
    /// @param branchId Идентификатор отделения.
    /// @return Ссылка на последнее событие указанного отделения.
    /// @note Добавление нового события может сделать ранее полученную ссылку недействительной.
    const TrackingEvent &findLastAtBranch(int branchId) const;
};

#endif
