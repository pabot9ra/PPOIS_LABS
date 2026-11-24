/// @file deliverynotice.h
/// @author Владислав Вертинский
/// @brief Уведомление о завершённой доставке.

#ifndef DELIVERYNOTICE_H
#define DELIVERYNOTICE_H

#include "postalbranch.h"
#include <string>

/// @brief Уведомление о завершённой доставке.
/// @details Хранит снимок имени получателя и адреса отделения. Отправка и прочтение представлены только отметками модели.
class DeliveryNotice {
private:
    int id_; ///< Идентификатор объекта.
    std::string trackingNumber_; ///< Трек-номер связанного отправления.
    std::string recipientName_; ///< Имя получателя на момент создания уведомления.
    std::string contact_; ///< Указанный контакт получателя.
    int branchId_; ///< Идентификатор отделения.
    std::string branchAddress_; ///< Строка адреса отделения на момент создания уведомления.
    bool sent_; ///< Отметка об отправке в модели.
    bool read_; ///< Отметка о прочтении в модели.

public:
    /// @brief Создаёт непрочитанное и неотправленное уведомление для доставленного отправления.
    /// @param id Идентификатор записи.
    /// @param branch Отделение, с которым связана операция.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @param contact Непустое контактное значение получателя.
    DeliveryNotice(int id, const PostalBranch &branch, const std::string &trackingNumber,
                   const std::string &contact);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Трек-номер отправления.
    /// @return Трек-номер отправления.
    const std::string &trackingNumber() const;
    /// @brief Указанный контакт получателя без проверки существования адреса или номера.
    /// @return Указанный контакт получателя без проверки существования адреса или номера.
    const std::string &contact() const;
    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Признак отметки уведомления отправленным.
    /// @return Признак отметки уведомления отправленным.
    bool sent() const;
    /// @brief Признак отметки уведомления прочитанным.
    /// @return Признак отметки уведомления прочитанным.
    bool read() const;
    /// @brief Формирует текст уведомления с получателем, трек-номером и адресом отделения.
    /// @return Текст уведомления о доставке.
    std::string formatMessage() const;
    /// @brief Отмечает уведомление отправленным без реальной рассылки.
    void markSent();
    /// @brief Отмечает отправленное уведомление прочитанным.
    void markRead();
};

#endif
