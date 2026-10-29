#include "menu.h"
#include "cube.h"
#include <iostream>
#include <fstream>
void runMenu() {
    Cube cube;
    int choice;
    while (true) {
        std::cout << "1. Загрузить кубик из файла\n"
                  << "2. Показать кубик\n"
                  << "3. Проверить собран ли кубик\n"
                  << "4. Перемешать кубик\n"
                  << "5. Сделать поворот\n"
                  << "6. Сбросить кубик\n"
                  << "0. Выход\n";
    
    std::cin >> choice;
    switch (choice) {
    case 1:{
        std::cout << "Введите путь к файлу: " << '\n';
        std::string path;
        std::cin >> path;
        std::ifstream file(path);
        if(!file){
            std::cerr << "Путь к файлу передан неверно!" << '\n';
            break;
        }
        Cube tmpcube;
        file >> tmpcube;
        if(!file){
            std::cerr << "Кубик передан неверно!" << '\n';
            break;
        }
        cube = tmpcube;
        break;
    }
    case 2:
        std::cout << cube;
        break;
    case 3:
        if(cube.isSolved()){
            std::cout << "Кубик собран" << '\n';
        } else {
            std::cout << "Кубик не собран" << '\n';
        }
        break;
    case 4:
        cube.mix(25);
        std::cout << "Кубик перемешан" << '\n';
        break;
    case 5:{
        std::cout << "Выберите какую грань хотите повернуть: \n"
                  << "1. Верхнюю\n"
                  << "2. Левую\n"
                  << "3. Переднюю\n"
                  << "4. Правую\n"
                  << "5. Заднюю\n"
                  << "6. Нижнюю\n";
        
        int facechoice;
        std::cin >> facechoice;
        if(!std::cin){
            std::cerr << "Нет такого варианта номера грани" << '\n';
            break;
        }
        facechoice--;
        int turnchoice;
        std::cout << "Выберите какой ход сделать: \n"
                  << "1. По часовой\n"
                  << "2. Против часовой\n"
                  << "3. Двойной по часовой\n";
        std::cin >> turnchoice;
        if(!std::cin){
            std::cerr << "Нет такой вариации хода" << '\n';
            break;
        }
        turnchoice--;
        cube.move(static_cast<FaceId>(facechoice), static_cast<Turn>(turnchoice));
        break;
    }
    case 6:{
        Cube startfaces;
        cube = startfaces;
        std::cout << "Кубик сброшен к собранному состоянию" << '\n';
        break;
    }
    default:{
        std::cerr << "Нет такого пункта!" << '\n';
        break;
    }
    case 0:{
        return;
    }
    }
}
}