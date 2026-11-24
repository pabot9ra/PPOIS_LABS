/// @file menu.h
/// @author Владислав Вертинский
/// @brief Консольное меню почтовой системы.

#ifndef MENU_H
#define MENU_H

#include <iosfwd>

/// @brief Запускает интерактивное меню с отдельной почтовой сетью в памяти.
/// @param input Поток пользовательского ввода.
/// @param output Поток вывода меню или примеров.
/// @return Ноль при выборе выхода или завершении потока ввода.
int runMenu(std::istream &input, std::ostream &output);

#endif
