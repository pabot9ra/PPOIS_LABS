/// @file demos.h
/// @author Владислав Вертинский
/// @brief Демонстрационные почтовые сценарии.

#ifndef DEMOS_H
#define DEMOS_H

#include <iosfwd>

/// @brief Выполняет все демонстрационные почтовые сценарии.
/// @param output Поток вывода меню или примеров.
void runDemonstrations(std::ostream &output);
/// @brief Выполняет выбранный пример на отдельных демонстрационных данных.
/// @param number Номер сценария от 1 до 5.
/// @param output Поток вывода меню или примеров.
void runDemonstration(int number, std::ostream &output);

#endif
