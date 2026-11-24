/// @file queueticket.h
/// @author Владислав Вертинский
/// @brief Талон клиента на услугу в течение одного рабочего дня.

#ifndef QUEUETICKET_H
#define QUEUETICKET_H

#include "customer.h"
#include <string>

/// @brief Состояние талона обслуживания.
enum class TicketStatus {
    Waiting, ///< Ожидает вызова.
    Called, ///< Вызван к окну.
    Served, ///< Обслуживание завершено.
    Cancelled ///< Отменён до вызова.
};

/// @brief Талон клиента на услугу в течение одного рабочего дня.
/// @details Отмена разрешена только до вызова. После вызова талон закреплён за окном обслуживания.
class QueueTicket {
private:
    int number_; ///< Номер талона.
    Customer customer_; ///< Собственная копия клиента талона.
    std::string service_; ///< Запрошенная услуга.
    int issuedMinute_; ///< Минута выдачи талона от начала суток.
    TicketStatus status_; ///< Текущее состояние объекта.
    int counterId_; ///< Назначенное окно; ноль до вызова.

public:
    /// @brief Создаёт ожидающий талон без назначенного окна.
    /// @param number Положительный номер талона.
    /// @param customer Клиент, данные которого используются в операции.
    /// @param service Непустое название почтовой услуги.
    /// @param issuedMinute Минута выдачи талона от начала суток.
    QueueTicket(int number, const Customer &customer, const std::string &service, int issuedMinute);

    /// @brief Номер талона.
    /// @return Номер талона.
    int number() const;
    /// @brief Данные клиента.
    /// @return Данные клиента.
    const Customer &customer() const;
    /// @brief Название запрошенной услуги.
    /// @return Название запрошенной услуги.
    const std::string &service() const;
    /// @brief Минута выдачи талона от начала суток.
    /// @return Минута выдачи талона от начала суток.
    int issuedMinute() const;
    /// @brief Текущее состояние объекта.
    /// @return Текущее состояние объекта.
    TicketStatus status() const;
    /// @brief Номер окна, вызвавшего талон; ноль до вызова.
    /// @return Номер окна, вызвавшего талон; ноль до вызова.
    int counterId() const;
    /// @brief Закрепляет ожидающий талон за положительным ID окна.
    /// @param counterId Положительный ID окна обслуживания.
    void call(int counterId);
    /// @brief Завершает обслуживание вызванного талона.
    void complete();
    /// @brief Отменяет только ожидающий талон.
    void cancel();
};

#endif
