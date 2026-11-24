/// @file accesspolicy.h
/// @author Владислав Вертинский
/// @brief Набор разрешений и учёт отказов в доступе.

#ifndef ACCESSPOLICY_H
#define ACCESSPOLICY_H

#include <cstddef>
#include <set>
#include <string>

/// @brief Набор разрешений и учёт отказов в доступе.
/// @details Отключение политики сохраняет выданные разрешения. Проверка запрещённой операции увеличивает счётчик отказов.
class AccessPolicy {
private:
    std::set<std::string> permissions_; ///< Множество выданных разрешений.
    bool enabled_; ///< Разрешено ли применение политики.
    std::size_t deniedCount_; ///< Число отказов при проверке доступа.

public:
    /// @brief Создаёт включённую политику без разрешений.
    AccessPolicy();

    /// @brief Признак включённой политики доступа.
    /// @return Признак включённой политики доступа.
    bool enabled() const;
    /// @brief Число отказов при проверке разрешений.
    /// @return Число отказов при проверке разрешений.
    std::size_t deniedCount() const;
    /// @brief Проверяет наличие выданного разрешения независимо от включённости политики.
    /// @param permission Название проверяемого или изменяемого разрешения.
    /// @return true, если разрешение выдано, даже при отключённой политике; иначе false.
    bool hasPermission(const std::string &permission) const;
    /// @brief Выдаёт разрешение; повторная выдача не создаёт дубликат.
    /// @param permission Название проверяемого или изменяемого разрешения.
    void grantPermission(const std::string &permission);
    /// @brief Отзывает ранее выданное разрешение.
    /// @param permission Название проверяемого или изменяемого разрешения.
    void revokePermission(const std::string &permission);
    /// @brief Проверяет включённость политики и наличие разрешения; при отказе увеличивает счётчик.
    /// @param permission Название проверяемого или изменяемого разрешения.
    /// @throws AccessDeniedException Политика отключена или разрешение не выдано.
    void checkPermission(const std::string &permission);
    /// @brief Отключает политику, сохраняя список разрешений.
    void disable();
    /// @brief Включает ранее отключённую политику.
    void enable();
};

#endif
