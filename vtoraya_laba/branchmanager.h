/// @file branchmanager.h
/// @author Владислав Вертинский
/// @brief Руководитель отделения, управляющий назначениями и сменами.

#ifndef BRANCHMANAGER_H
#define BRANCHMANAGER_H

#include "employee.h"
#include "accesspolicy.h"
#include <cstddef>

/// @brief Руководитель отделения, управляющий назначениями и сменами.
/// @details Правила доступа унаследованы как protected; управление требует собственной смены в нужном отделении.
class BranchManager : public Employee, protected AccessPolicy {
private:
    std::size_t assignedCount_; ///< Число успешных назначений.
    std::size_t startedShifts_; ///< Число открытых смен сотрудников.
    std::size_t endedShifts_; ///< Число закрытых смен сотрудников.

    /// @brief Проверяет собственную смену, разрешение и запрет управления собой через служебную операцию.
    /// @param branch Отделение, с которым связана операция.
    /// @param employee Сотрудник, участвующий в операции.
    /// @param permission Название проверяемого или изменяемого разрешения.
    void ensureCanManage(const PostalBranch &branch, const Employee &employee,
                         const std::string &permission);

public:
    /// @brief Создаёт руководителя с разрешениями назначения сотрудников и управления сменами.
    /// @param id Идентификатор записи.
    /// @param name Непустое имя или название.
    /// @param branchId Идентификатор отделения.
    BranchManager(int id, const std::string &name, int branchId);

    /// @copydoc AccessPolicy::enabled
    using AccessPolicy::enabled;
    /// @copydoc AccessPolicy::deniedCount
    using AccessPolicy::deniedCount;

    /// @brief Название почтовой роли сотрудника.
    /// @return Название почтовой роли сотрудника.
    std::string role() const override;
    /// @brief Число успешных назначений сотрудников.
    /// @return Число успешных назначений сотрудников.
    std::size_t assignedCount() const;
    /// @brief Число открытых руководителем смен сотрудников.
    /// @return Число открытых руководителем смен сотрудников.
    std::size_t startedShifts() const;
    /// @brief Число закрытых руководителем смен сотрудников.
    /// @return Число закрытых руководителем смен сотрудников.
    std::size_t endedShifts() const;
    /// @brief Назначает другого сотрудника в своё отделение.
    /// @param branch Отделение, с которым связана операция.
    /// @param employee Сотрудник, участвующий в операции.
    void assignEmployee(const PostalBranch &branch, Employee &employee);
    /// @brief Открывает смену сотрудника своего отделения.
    /// @param branch Отделение, с которым связана операция.
    /// @param employee Сотрудник, участвующий в операции.
    void startEmployeeShift(const PostalBranch &branch, Employee &employee);
    /// @brief Закрывает смену сотрудника своего отделения.
    /// @param branch Отделение, с которым связана операция.
    /// @param employee Сотрудник, участвующий в операции.
    void endEmployeeShift(const PostalBranch &branch, Employee &employee);
    /// @brief Временно запрещает операции управления.
    void suspendManagement();
    /// @brief Восстанавливает разрешённые операции управления.
    void restoreManagement();
};

#endif
