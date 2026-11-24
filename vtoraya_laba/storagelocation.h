/// @file storagelocation.h
/// @author Владислав Вертинский
/// @brief Адрес ячейки: отделение, зона и номер ячейки.

#ifndef STORAGELOCATION_H
#define STORAGELOCATION_H

#include <string>

/// @brief Адрес ячейки: отделение, зона и номер ячейки.
/// @details Самостоятельное значение; не хранит ссылку на склад и не меняет занятость ячейки.
class StorageLocation {
private:
    int branchId_; ///< Идентификатор отделения.
    std::string zoneId_; ///< ID зоны.
    std::string cellId_; ///< ID ячейки.

public:
    /// @brief Создаёт адрес хранения.
    /// @param branchId Идентификатор отделения.
    /// @param zoneId Непустой ID зоны без пробелов.
    /// @param cellId Непустой ID ячейки без пробелов.
    StorageLocation(int branchId, const std::string &zoneId, const std::string &cellId);

    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Идентификатор зоны.
    /// @return Идентификатор зоны.
    const std::string &zoneId() const;
    /// @brief Идентификатор ячейки.
    /// @return Идентификатор ячейки.
    const std::string &cellId() const;
    /// @brief Формирует адрес хранения в виде «отделение/зона/ячейка».
    /// @return Текстовое представление записи.
    std::string format() const;
    /// @brief Сравнивает отделение, зону и ячейку двух адресов хранения.
    /// @param other Другой объект для сравнения или копирования.
    /// @return true при совпадении отделения, зоны и ячейки; иначе false.
    bool operator==(const StorageLocation &other) const;
};

#endif
