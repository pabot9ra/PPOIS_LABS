/// @file servicecounter.h
/// @author Владислав Вертинский
/// @brief Окно обслуживания со сменой сотрудника и текущим талоном.

#ifndef SERVICECOUNTER_H
#define SERVICECOUNTER_H

#include "workinghours.h"
#include "servicequeue.h"
#include "employee.h"

/// @brief Окно обслуживания со сменой сотрудника и текущим талоном.
/// @details Новая операция должна помещаться в рабочий интервал без перерыва. Уже начатое обслуживание можно завершить позже.
class ServiceCounter {
private:
    int id_; ///< Идентификатор объекта.
    int branchId_; ///< Идентификатор отделения.
    WorkingHours hours_; ///< Расписание окна.
    bool open_; ///< Открыта ли смена или окно.
    int employeeId_; ///< Назначенный сотрудник; ноль для закрытого окна.
    int currentTicket_; ///< Текущий талон; ноль при отсутствии.
    int currentQueueId_; ///< Очередь текущего талона.

    /// @brief Проверяет открытость окна и смену назначенного сотрудника в нужном отделении.
    /// @param employee Сотрудник, участвующий в операции.
    void ensureStaff(const Employee &employee) const;
    /// @brief Завершает вызванный талон либо принимает уже завершённую запись очереди.
    /// @param queue Очередь талонов указанного отделения.
    void finishTicket(ServiceQueue &queue);

public:
    /// @brief Создаёт закрытое окно без назначенного сотрудника и талона.
    /// @param id Идентификатор записи.
    /// @param branch Отделение, с которым связана операция.
    /// @param hours Расписание окна с одним возможным перерывом.
    ServiceCounter(int id, const PostalBranch &branch, const WorkingHours &hours);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Расписание окна обслуживания.
    /// @return Расписание окна обслуживания.
    const WorkingHours &hours() const;
    /// @brief Признак открытой смены или окна.
    /// @return Признак открытой смены или окна.
    bool isOpen() const;
    /// @brief ID назначенного сотрудника; ноль для закрытого окна.
    /// @return ID назначенного сотрудника; ноль для закрытого окна.
    int employeeId() const;
    /// @brief Номер текущего талона; ноль, если обслуживание не начато.
    /// @return Номер текущего талона; ноль, если обслуживание не начато.
    int currentTicket() const;
    /// @brief Открывает окно для сотрудника своего отделения в рабочее время.
    /// @param employee Сотрудник, участвующий в операции.
    /// @param minute Минута от начала суток, от 0 до 1439.
    void open(const Employee &employee, int minute);
    /// @brief Вызывает следующий талон после проверки сотрудника, очереди и времени на обслуживание.
    /// @param queue Очередь талонов указанного отделения.
    /// @param employee Сотрудник, участвующий в операции.
    /// @param minute Минута от начала суток, от 0 до 1439.
    /// @param expectedMinutes Положительная ожидаемая длительность обслуживания.
    /// @return Номер вызванного талона.
    int callNext(ServiceQueue &queue, const Employee &employee, int minute, int expectedMinutes);
    /// @brief Завершает обслуживание своего талона и освобождает окно.
    /// @param queue Очередь талонов указанного отделения.
    /// @param employee Сотрудник, участвующий в операции.
    void finishCurrent(ServiceQueue &queue, const Employee &employee);
    /// @brief Закрывает окно после завершения текущего талона.
    /// @param employee Сотрудник, участвующий в операции.
    void close(const Employee &employee);
};

#endif
