/// @file accessdeniedexception.h
/// @author Владислав Вертинский
/// @brief Отказ в операции с сохранением названия разрешения.

#ifndef ACCESSDENIEDEXCEPTION_H
#define ACCESSDENIEDEXCEPTION_H

#include "postalexception.h"

/// @brief Отказ в операции с сохранением названия разрешения.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class AccessDeniedException : public PostalException {
private:
    std::string permission_; ///< Разрешение, в котором отказано.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param permission Название проверяемого или изменяемого разрешения.
    explicit AccessDeniedException(const std::string &permission);

    /// @brief Название запрещённого разрешения.
    /// @return Название запрещённого разрешения.
    const std::string &permission() const;
};

#endif
