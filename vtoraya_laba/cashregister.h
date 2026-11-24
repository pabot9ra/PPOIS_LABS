/// @file cashregister.h
/// @author Владислав Вертинский
/// @brief Касса с платежами и документами возврата.

#ifndef CASHREGISTER_H
#define CASHREGISTER_H

#include "receipt.h"
#include "refund.h"
#include "shipment.h"
#include <string>
#include <vector>

/// @brief Касса с платежами и документами возврата.
/// @details Все суммы в копейках. Баланс равен учтённым платежам за вычетом возвратов. Записи сохраняются между сменами.
class CashRegister {
private:
    int id_; ///< Идентификатор объекта.
    int branchId_; ///< Идентификатор отделения.
    int balance_; ///< Баланс учтённых платежей, коп.
    bool open_; ///< Открыта ли смена или окно.
    std::vector<Payment> payments_; ///< Учтённые платежи.
    std::vector<Refund> processedRefunds_; ///< Обработанные документы возврата.

    /// @brief Проверяет, что смена объекта открыта.
    void ensureOpen() const;
    /// @brief Проверяет уникальность ID платежа и отсутствие невозвращённой оплаты этого отправления.
    /// @param paymentId Идентификатор платежа.
    /// @param trackingNumber Трек-номер без пробельных символов.
    void ensureUnpaid(int paymentId, const std::string &trackingNumber) const;
    /// @brief Находит платёж для внутреннего изменения.
    /// @param paymentId Идентификатор платежа.
    /// @return Находит платёж для внутреннего изменения.
    Payment &findMutablePayment(int paymentId);

public:
    /// @brief Создаёт закрытую кассу с нулевым балансом.
    /// @param id Идентификатор записи.
    /// @param branchId Идентификатор отделения.
    CashRegister(int id, int branchId);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Учтённый баланс кассы в копейках.
    /// @return Учтённый баланс кассы в копейках.
    int balance() const;
    /// @brief Признак открытой смены или окна.
    /// @return Признак открытой смены или окна.
    bool isOpen() const;
    /// @brief Открывает смену объекта.
    void openShift();
    /// @brief Закрывает открытую смену объекта.
    void closeShift();
    /// @brief Проводит оплату по полиморфной стоимости отправления и формирует чек.
    /// @param paymentId Идентификатор платежа.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @return Чек проведённого платежа.
    Receipt acceptPayment(int paymentId, const Shipment &shipment);
    /// @brief Находит проведённый платёж по ID.
    /// @param paymentId Идентификатор платежа.
    /// @return Ссылка на проведённый платёж.
    /// @note Добавление нового платежа может сделать ранее полученную ссылку недействительной.
    /// @throws PaymentNotFoundException Искомая запись не найдена.
    const Payment &findPayment(int paymentId) const;
    /// @brief Находит обработанный документ возврата по ID.
    /// @param refundId ID возврата.
    /// @return Ссылка на документ обработанного возврата.
    /// @note Добавление нового возврата может сделать ранее полученную ссылку недействительной.
    const Refund &findRefund(int refundId) const;
    /// @brief Учитывает документ возврата однократно и сохраняет его причину.
    /// @param refund Документ возврата с суммой и причиной.
    /// @throws PaymentNotFoundException Исходный платёж не найден.
    /// @throws RefundLimitException Превышен допустимый остаток.
    /// @throws std::logic_error Смена кассы закрыта.
    /// @throws std::invalid_argument Возврат уже обработан.
    void processRefund(const Refund &refund);
};

#endif
