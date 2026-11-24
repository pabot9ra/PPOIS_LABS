/// @file postalexception.h
/// @author Владислав Вертинский
/// @brief Общая база собственных ошибок почтовой системы.

#ifndef POSTALEXCEPTION_H
#define POSTALEXCEPTION_H

#include <stdexcept>
#include <string>

/// @brief Общая база собственных ошибок почтовой системы.
/// @details Наследуется от std::runtime_error; позволяет перехватывать доменные ошибки одним обработчиком.
class PostalException : public std::runtime_error {
public:
    /// @brief Создаёт доменную ошибку с заданным сообщением.
    /// @param message Поясняющее сообщение об ошибке.
    explicit PostalException(const std::string &message);
};

#endif
