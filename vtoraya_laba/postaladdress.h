/// @file postaladdress.h
/// @author Владислав Вертинский
/// @brief Почтовый адрес с шестизначным индексом.

#ifndef POSTALADDRESS_H
#define POSTALADDRESS_H

#include <string>

/// @brief Почтовый адрес с шестизначным индексом.
/// @details Проверяется формат индекса, а не его существование или соответствие улице. Поля адреса не могут быть пустыми.
class PostalAddress {
private:
    std::string city_; ///< Населённый пункт.
    std::string street_; ///< Улица.
    std::string house_; ///< Номер дома строкой.
    std::string postalCode_; ///< Шестизначный индекс.

public:
    /// @brief Создаёт и проверяет адрес.
    /// @param city Населённый пункт; не только пробельные символы.
    /// @param street Улица; не только пробельные символы.
    /// @param house Номер дома строкой, в том числе с буквами.
    /// @param postalCode Ровно шесть цифр ASCII.
    /// @throws InvalidAddressException Обязательное поле пустое или индекс имеет неверный формат.
    PostalAddress(const std::string &city, const std::string &street,
                  const std::string &house, const std::string &postalCode);

    /// @brief Населённый пункт.
    /// @return Населённый пункт.
    const std::string &city() const;
    /// @brief Название улицы.
    /// @return Название улицы.
    const std::string &street() const;
    /// @brief Номер дома, включая возможные буквы.
    /// @return Номер дома, включая возможные буквы.
    const std::string &house() const;
    /// @brief Шестизначный индекс с сохранением ведущих нулей.
    /// @return Шестизначный индекс с сохранением ведущих нулей.
    const std::string &postalCode() const;

    /// @brief Формирует строку адреса для почтовой этикетки.
    /// @return Строка почтового адреса для этикетки.
    std::string formatLabel() const;
};

#endif
