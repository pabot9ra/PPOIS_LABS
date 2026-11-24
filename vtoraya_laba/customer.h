/// @file customer.h
/// @author Владислав Вертинский
/// @brief Клиент почты с контактным именем и адресом.

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "postaladdress.h"
#include <string>

/// @brief Клиент почты с контактным именем и адресом.
/// @details Адрес хранится собственной копией. ID должен быть положительным, имя не должно состоять только из пробелов.
class Customer {
private:
    int id_; ///< Идентификатор объекта.
    std::string name_; ///< Имя или название объекта.
    PostalAddress address_; ///< Собственная копия адреса.

public:
    /// @brief Создаёт клиента и проверяет ID и имя.
    /// @param id Идентификатор записи.
    /// @param name Непустое имя или название.
    /// @param address Почтовый адрес, сохраняемый копией.
    /// @throws InvalidCustomerException ID неположительный или имя пустое.
    Customer(int id, const std::string &name, const PostalAddress &address);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Имя клиента.
    /// @return Имя клиента.
    const std::string &name() const;
    /// @brief Адрес объекта.
    /// @return Адрес объекта.
    const PostalAddress &address() const;

    /// @brief Заменяет адрес клиента собственной копией нового адреса.
    /// @param address Почтовый адрес, сохраняемый копией.
    void changeAddress(const PostalAddress &address);
};

#endif
