/// @file branchnotfoundexception.h
/// @author Владислав Вертинский
/// @brief Ошибка поиска отделения по ID.

#ifndef BRANCHNOTFOUNDEXCEPTION_H
#define BRANCHNOTFOUNDEXCEPTION_H

#include "postalexception.h"

/// @brief Ошибка поиска отделения по ID.
/// @details Данные ошибочного запроса сохраняются для диагностики; корректность самого контекста ошибки не проверяется.
class BranchNotFoundException : public PostalException {
private:
    int branchId_; ///< Идентификатор отделения.

public:
    /// @brief Создаёт исключение с диагностическим контекстом.
    /// @param branchId Искомый ID отделения.
    explicit BranchNotFoundException(int branchId);

    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
};

#endif
