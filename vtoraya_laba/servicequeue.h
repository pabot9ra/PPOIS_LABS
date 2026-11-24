/// @file servicequeue.h
/// @author Владислав Вертинский
/// @brief Очередь талонов одного отделения на один день.

#ifndef SERVICEQUEUE_H
#define SERVICEQUEUE_H

#include "queueticket.h"
#include "postalbranch.h"
#include <cstddef>
#include <vector>

/// @brief Очередь талонов одного отделения на один день.
/// @details Выдача хронологическая, вызов ожидающих — FIFO. Закрытие выдачи не запрещает обслуживать уже выданные талоны.
class ServiceQueue {
private:
    int id_; ///< Идентификатор объекта.
    int branchId_; ///< Идентификатор отделения.
    int nextNumber_; ///< Следующий номер талона.
    std::vector<QueueTicket> tickets_; ///< Записи выданных талонов.
    bool issuingOpen_; ///< Разрешена ли выдача талонов.

    /// @brief Находит талон для внутреннего изменения.
    /// @param number Положительный номер талона.
    /// @return Находит талон для внутреннего изменения.
    QueueTicket &findMutableTicket(int number);

public:
    /// @brief Создаёт пустую очередь с открытой выдачей талонов.
    /// @param id Идентификатор записи.
    /// @param branch Отделение, с которым связана операция.
    ServiceQueue(int id, const PostalBranch &branch);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Признак открытой выдачи новых талонов.
    /// @return Признак открытой выдачи новых талонов.
    bool issuingOpen() const;
    /// @brief Число ожидающих талонов без вызванных и отменённых.
    /// @return Число ожидающих талонов без вызванных и отменённых.
    std::size_t waitingCount() const;
    /// @brief Выдаёт следующий талон, сохраняя хронологический порядок выдачи.
    /// @param customer Клиент, данные которого используются в операции.
    /// @param service Непустое название почтовой услуги.
    /// @param minute Минута от начала суток, от 0 до 1439.
    /// @return Номер выданного талона; нумерация локальна для очереди.
    int issue(const Customer &customer, const std::string &service, int minute);
    /// @brief Находит талон по его локальному номеру.
    /// @param number Положительный номер талона.
    /// @return Ссылка на талон очереди.
    /// @note Выдача нового талона может сделать ранее полученную ссылку недействительной.
    const QueueTicket &findTicket(int number) const;
    /// @brief Вызывает первый доступный ожидающий талон для свободного окна.
    /// @param counterId Положительный ID окна обслуживания.
    /// @param minute Минута от начала суток, от 0 до 1439.
    /// @return Номер вызванного талона.
    int callNext(int counterId, int minute);
    /// @brief Завершает вызванный талон только для закреплённого за ним окна.
    /// @param number Положительный номер талона.
    /// @param counterId Положительный ID окна обслуживания.
    void completeTicket(int number, int counterId);
    /// @brief Отменяет ожидающий талон.
    /// @param number Положительный номер талона.
    void cancelTicket(int number);
    /// @brief Закрывает выдачу новых талонов, сохраняя обслуживание имеющихся.
    void closeIssuing();
    /// @brief Возобновляет выдачу новых талонов.
    void openIssuing();
};

#endif
