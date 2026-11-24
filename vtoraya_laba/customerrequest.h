/// @file customerrequest.h
/// @author Владислав Вертинский
/// @brief Абстрактная заявка участника отправления.

#ifndef CUSTOMERREQUEST_H
#define CUSTOMERREQUEST_H

#include "shipment.h"
#include <memory>
#include <string>

/// @brief Состояние обработки клиентской заявки.
enum class RequestStatus {
    Open, ///< Подана, решение не принято.
    Approved, ///< Одобрена к исполнению.
    Rejected, ///< Отклонена с причиной.
    Completed ///< Заявка исполнена: выдан ярлык или учтена компенсация.
};

/// @brief Абстрактная заявка участника отправления.
/// @details Одобрение и отказ разрешены только для открытой заявки. Исполнение выполняет конкретный наследник.
class CustomerRequest {
private:
    int id_; ///< Идентификатор объекта.
    Customer applicant_; ///< Собственная копия заявителя.
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    int openedMinute_; ///< Минута создания заявки.
    std::string reason_; ///< Причина операции.
    RequestStatus status_; ///< Текущее состояние объекта.
    std::string decisionReason_; ///< Причина отказа по заявке.

protected:
    /// @brief Копирует состояние для создания независимого наследника.
    /// @param other Другой объект для сравнения или копирования.
    CustomerRequest(const CustomerRequest &other) = default;
    /// @brief Проверяет наличие хотя бы одного непробельного символа.
    /// @param value Проверяемая строка.
    /// @return true, если строка содержит непробельный символ; иначе false.
    static bool hasText(const std::string &value);
    /// @brief Проверяет трек-номер и участие заявителя в отправлении.
    /// @param shipment Отправление, над которым выполняется операция.
    void ensureMatches(const Shipment &shipment) const;
    /// @brief Проверяет, что заявка одобрена.
    void ensureApproved() const;
    /// @brief Переводит одобренную заявку в исполненное состояние.
    void markCompleted();
    /// @brief Проверяет условия одобрения для конкретного вида заявки.
    /// @param shipment Отправление, над которым выполняется операция.
    virtual void validateEligibility(const Shipment &shipment) const = 0;

public:
    /// @brief Создаёт открытую заявку отправителя или получателя.
    /// @param id Идентификатор записи.
    /// @param applicant Заявитель: отправитель или получатель указанного отправления.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param openedMinute Неотрицательная минута создания заявки.
    /// @param reason Непустое пояснение причины.
    CustomerRequest(int id, const Customer &applicant, const Shipment &shipment,
                    int openedMinute, const std::string &reason);
    /// @brief Освобождает объект при удалении через базовый указатель.
    virtual ~CustomerRequest() = default;
    /// @brief Запрещает частичное присваивание через базовый тип.
    /// @param other Другой объект для сравнения или копирования.
    CustomerRequest &operator=(const CustomerRequest &other) = delete;

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Сохранённая копия клиента-заявителя.
    /// @return Сохранённая копия клиента-заявителя.
    const Customer &applicant() const;
    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Минута создания заявки.
    /// @return Минута создания заявки.
    int openedMinute() const;
    /// @brief Причина операции.
    /// @return Причина операции.
    const std::string &reason() const;
    /// @brief Текущее состояние объекта.
    /// @return Текущее состояние объекта.
    RequestStatus status() const;
    /// @brief Причина отказа; пустая строка до отклонения.
    /// @return Причина отказа; пустая строка до отклонения.
    const std::string &decisionReason() const;
    /// @brief Одобряет открытую заявку после проверки актуального отправления.
    /// @param shipment Отправление, над которым выполняется операция.
    void approve(const Shipment &shipment);
    /// @brief Отклоняет открытую заявку с непустой причиной.
    /// @param reason Непустое пояснение причины.
    void reject(const std::string &reason);
    /// @brief Создаёт независимую полиморфную копию с сохранением конкретного типа и состояния.
    /// @return Уникальный указатель на независимую копию того же конкретного типа.
    virtual std::unique_ptr<CustomerRequest> clone() const = 0;
};

#endif
