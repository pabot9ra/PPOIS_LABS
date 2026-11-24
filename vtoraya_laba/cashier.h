/// @file cashier.h
/// @author Владислав Вертинский
/// @brief Сотрудник, работающий с закрытой реализацией кассы.

#ifndef CASHIER_H
#define CASHIER_H

#include "employee.h"
#include "cashregister.h"

/// @brief Сотрудник, работающий с закрытой реализацией кассы.
/// @details CashRegister унаследован как private. using выбирает ID сотрудника и открывает только чтение кассовых записей.
class Cashier : public Employee, private CashRegister {
public:
    /// @brief Связывает кассира с кассой одного отделения.
    /// @param id Идентификатор записи.
    /// @param name Непустое имя или название.
    /// @param branchId Идентификатор отделения.
    /// @param registerId Положительный идентификатор кассы.
    Cashier(int id, const std::string &name, int branchId, int registerId);

    /// @copydoc Employee::id
    using Employee::id;
    /// @copydoc Employee::branchId
    using Employee::branchId;
    /// @copydoc CashRegister::balance
    using CashRegister::balance;
    /// @copydoc CashRegister::findPayment
    using CashRegister::findPayment;
    /// @copydoc CashRegister::findRefund
    using CashRegister::findRefund;

    /// @brief Название почтовой роли сотрудника.
    /// @return Название почтовой роли сотрудника.
    std::string role() const override;
    /// @brief Открывает согласованную смену сотрудника и его кассы.
    void startShift() override;
    /// @brief Закрывает смену кассы и сотрудника.
    void endShift() override;
    /// @brief Разрешает назначение только в отделение закреплённой кассы и только вне смены.
    /// @param branchId Идентификатор отделения.
    void assignToBranch(int branchId) override;
    /// @brief Проводит оплату по полиморфной стоимости отправления и формирует чек.
    /// @param branch Отделение, с которым связана операция.
    /// @param paymentId Идентификатор платежа.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @return Чек проведённого платежа.
    Receipt acceptPayment(const PostalBranch &branch, int paymentId, const Shipment &shipment);
    /// @brief Учитывает документ возврата однократно и сохраняет его причину.
    /// @param branch Отделение, с которым связана операция.
    /// @param refund Документ возврата с суммой и причиной.
    void processRefund(const PostalBranch &branch, const Refund &refund);
};

#endif
