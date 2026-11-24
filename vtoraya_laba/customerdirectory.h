/// @file customerdirectory.h
/// @author Владислав Вертинский
/// @brief Справочник клиентов по уникальному ID.

#ifndef CUSTOMERDIRECTORY_H
#define CUSTOMERDIRECTORY_H

#include "customer.h"
#include <cstddef>
#include <map>

/// @brief Справочник клиентов по уникальному ID.
/// @details Хранит собственные копии клиентов. Смена адреса не изменяет ранее созданные отправления.
class CustomerDirectory {
private:
    std::map<int, Customer> customers_; ///< Зарегистрированные клиенты по ID.

public:
    /// @brief Число зарегистрированных записей.
    /// @return Число зарегистрированных записей.
    std::size_t size() const;
    /// @brief Проверяет наличие записи по идентификатору.
    /// @param id Идентификатор записи.
    /// @return true, если запись найдена; иначе false.
    bool contains(int id) const;
    /// @brief Регистрирует запись с уникальным ID.
    /// @param customer Клиент, данные которого используются в операции.
    void add(const Customer &customer);
    /// @brief Находит зарегистрированную запись по ID.
    /// @param id Идентификатор записи.
    /// @return Ссылка на запись с указанным ID.
    /// @note Ссылка сохраняется до удаления клиента или уничтожения справочника.
    /// @throws CustomerNotFoundException ID клиента не зарегистрирован.
    const Customer &find(int id) const;
    /// @brief Заменяет адрес клиента собственной копией нового адреса.
    /// @param id Идентификатор записи.
    /// @param address Почтовый адрес, сохраняемый копией.
    /// @throws CustomerNotFoundException Искомая запись не найдена.
    void changeAddress(int id, const PostalAddress &address);
    /// @brief Удаляет зарегистрированную запись по ID.
    /// @param id Идентификатор записи.
    /// @throws CustomerNotFoundException Искомая запись не найдена.
    void remove(int id);
};

#endif
