/// @file main.cpp
/// @brief Запуск меню или демонстраций по параметрам командной строки.

#include "menu.h"
#include "demos.h"
#include <exception>
#include <iostream>
#include <string>

/// @brief Запускает меню, примеры (--demo) или справку (--help).
/// @param argc Количество аргументов вместе с именем программы.
/// @param argv Аргументы командной строки.
/// @return 0 при успехе, 1 при исключении, 2 при неизвестных параметрах.
int main(int argc, char *argv[]) {
    try {
        if(argc == 1){
            return runMenu(std::cin, std::cout);
        }
        if(argc == 2 && std::string(argv[1]) == "--demo"){
            runDemonstrations(std::cout);
            return 0;
        }
        if(argc == 2 && std::string(argv[1]) == "--help"){
            std::cout << "postal — интерактивное меню\npostal --demo — примеры операций\n";
            return 0;
        }
        std::cerr << "Неизвестные параметры. Используйте postal --help\n";
        return 2;
    } catch(const std::exception &error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }
}
