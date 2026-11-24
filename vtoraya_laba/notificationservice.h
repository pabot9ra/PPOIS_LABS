/// @file notificationservice.h
/// @author Владислав Вертинский
/// @brief Учёт уведомлений одного отделения.

#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include "deliverynotice.h"
#include <cstddef>
#include <vector>

/// @brief Учёт уведомлений одного отделения.
/// @details Номера уведомлений локальны для сервиса. Реальная рассылка не выполняется.
class NotificationService {
private:
    int branchId_; ///< Идентификатор отделения.
    int nextId_; ///< Следующий локальный ID уведомления.
    std::vector<DeliveryNotice> notices_; ///< Созданные уведомления.

    /// @brief Находит уведомление для внутреннего изменения.
    /// @param id Идентификатор записи.
    /// @return Находит уведомление для внутреннего изменения.
    DeliveryNotice &findMutableNotice(int id);

public:
    /// @brief Создаёт сервис уведомлений указанного отделения.
    /// @param branch Отделение, с которым связана операция.
    explicit NotificationService(const PostalBranch &branch);

    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Число созданных уведомлений.
    /// @return Число созданных уведомлений.
    std::size_t noticeCount() const;
    /// @brief Создаёт уведомление для доставленного отправления своего отделения.
    /// @param branch Отделение, с которым связана операция.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param contact Непустое контактное значение получателя.
    /// @return Локальный ID созданного уведомления.
    int createNotice(const PostalBranch &branch, const std::string &trackingNumber,
                     const std::string &contact);
    /// @brief Находит уведомление по его локальному ID.
    /// @param id Идентификатор записи.
    /// @return Ссылка на сохранённое уведомление.
    /// @note Создание нового уведомления может сделать ранее полученную ссылку недействительной.
    const DeliveryNotice &findNotice(int id) const;
    /// @brief Отмечает уведомление отправленным без реальной рассылки.
    /// @param id Идентификатор записи.
    void markSent(int id);
    /// @brief Отмечает отправленное уведомление прочитанным.
    /// @param id Идентификатор записи.
    void markRead(int id);
};

#endif
