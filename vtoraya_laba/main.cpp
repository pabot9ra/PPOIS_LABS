#include "menu.h"
#include "demos.h"
#include <exception>
#include <iostream>
#include <string>

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
