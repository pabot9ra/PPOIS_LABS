/// @file requestregistry.h
/// @author Владислав Вертинский
/// @brief Реестр полиморфных заявок одного отделения.

#ifndef REQUESTREGISTRY_H
#define REQUESTREGISTRY_H

#include "customerrequest.h"
#include "postalbranch.h"
#include <cstddef>
#include <memory>
#include <vector>

/// @brief Реестр полиморфных заявок одного отделения.
/// @details Хранит собственные копии заявок. Перед исполнением повторно проверяет актуальное состояние отправления.
class RequestRegistry {
private:
    int branchId_; ///< Идентификатор отделения.
    std::vector<std::unique_ptr<CustomerRequest>> requests_; ///< Полиморфные копии заявок.

    /// @brief Проверяет принадлежность операции отделению реестра.
    /// @param branch Отделение, с которым связана операция.
    void ensureBranch(const PostalBranch &branch) const;
    /// @brief Находит заявку для внутреннего изменения.
    /// @param id Идентификатор записи.
    /// @return Находит заявку для внутреннего изменения.
    CustomerRequest &findMutableRequest(int id);

public:
    /// @brief Создаёт пустой реестр заявок отделения.
    /// @param branch Отделение, с которым связана операция.
    explicit RequestRegistry(const PostalBranch &branch);

    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Число зарегистрированных заявок.
    /// @return Число зарегистрированных заявок.
    std::size_t requestCount() const;
    /// @brief Сохраняет полиморфную копию открытой заявки с уникальным ID.
    /// @param branch Отделение, с которым связана операция.
    /// @param request Клиентская заявка.
    void submit(const PostalBranch &branch, const CustomerRequest &request);
    /// @brief Находит заявку по ID.
    /// @param id Идентификатор записи.
    /// @return Ссылка на сохранённую заявку.
    const CustomerRequest &findRequest(int id) const;
    /// @brief Одобряет открытую заявку после проверки актуального отправления.
    /// @param id Идентификатор записи.
    /// @param branch Отделение, с которым связана операция.
    void approve(int id, const PostalBranch &branch);
    /// @brief Отклоняет открытую заявку с непустой причиной.
    /// @param id Идентификатор записи.
    /// @param reason Непустое пояснение причины.
    void reject(int id, const std::string &reason);
    /// @brief Выдаёт возвратный ярлык для одобренной заявки.
    /// @param id Идентификатор записи.
    /// @param branch Отделение, с которым связана операция.
    /// @return Готовый возвратный ярлык.
    std::string issueReturnLabel(int id, const PostalBranch &branch);
    /// @brief Выдаёт ярлык переадресации для одобренной заявки.
    /// @param id Идентификатор записи.
    /// @param branch Отделение, с которым связана операция.
    /// @return Готовый ярлык нового адреса.
    std::string issueRedirectionLabel(int id, const PostalBranch &branch);
    /// @brief Исполняет одобренную страховую претензию с проверкой остатка покрытия.
    /// @param id Идентификатор записи.
    /// @param branch Отделение, с которым связана операция.
    void settleDamageClaim(int id, PostalBranch &branch);
};

#endif
