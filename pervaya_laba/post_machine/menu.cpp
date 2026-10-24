#include "menu.h"
#include <iostream>
#include <fstream>
#include <string>
#include "postmachine.h"
#include <limits>

void runMenu(){
    int choice;
    PostMachine machine;
    while(true){
        std::cout  <<  "1. Загрузить машину из файла\n"
                   <<  "2. Показать машину\n" 
                   <<  "3. Показать программу\n"
                   <<  "4. Добавить команду\n"
                   <<  "5. Удалить команду\n"
                   <<  "6. Задать ленту\n"
                   <<  "7. Задать позицию каретки\n"
                   <<  "8. Сделать шаг\n"
                   <<  "9. Выполнить до конца\n"
                   <<  "10. Сбросить\n"
                   <<  "0. Выход\n";
    
    std::cin >> choice;
    if(!std::cin){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Введите число!" << '\n';
        continue;
    }
    switch(choice){
    case 1:{
        std::cout << "Введите путь к файлу" << '\n';
        std::string path;
        std::cin >> path;
        std::ifstream file(path);
        if(!file){
            std::cerr << "Не удалось открыть файл!" << '\n';
            break;
        }
        PostMachine tmpmachine;
        file >> tmpmachine;
        if(!file){
            std::cerr << "Машина Поста передана неправильно!" << '\n';
            break;
        }
        machine = tmpmachine;
        break;
    }
    case 2:
        std::cout << machine;
        break;
    case 3:
        std::cout << machine.program();
        break;
    case 4:{
        Instruction ins;
        std::cout << "Введите команду" << '\n';
        std::cin >> ins;
        if(!std::cin){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Неверная команда! Формат: ? 2, 5; V 3; !" << '\n';
        continue;
        }
        Program tmpprog = machine.program();
        tmpprog.add(ins);
        machine.setProgram(tmpprog);
        break;
    }
    case 5:{
        int num;
        std::cout << "Введите номер команды для удаления" << '\n';
        std::cin >> num;
        if(!std::cin){
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::cerr << "Введите номер команды!" << '\n';
             continue;
        }
        Program tmpprogram = machine.program();
        if(!tmpprogram.remove(num)){
            std::cerr << "Нет команды с номером " << num << "!"  << '\n';
            continue;
        }
        machine.setProgram(tmpprogram);
        break;
    }
    case 6:{
        Tape tape;
        std::cout << "Введите ленту в формате: 2 VV.V" << '\n';
        std::cin >> tape;
        if(!std::cin){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Лента введена неверно!" << '\n';
            continue;
        }
        machine.setTape(tape);
        break;
    }
    case 7:{
        std::cout << "Введите позицию!" << '\n';
        long long pos;
        std::cin >> pos;
        if(!std::cin){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Введите позицию(число)!" << '\n';
            continue;
        }
        machine.setPosition(pos);
        break;
    }
    case 8:
        if(!machine.step()){
            std::cerr << "Машина не работает!" << '\n';
        }
        std::cout << machine;
        break;
    case 9:{
        int limit;
        std::cout << "Введите максимальное число шагов" << '\n';
        std::cin >> limit;
        if(!std::cin){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Введите ЧИСЛО шагов!" << '\n';
            continue;
        }
        std::cout << "Число шагов: " << machine.run(limit) << '\n';
        std::cout << machine;

        if(machine.state() == State::Running){
            std::cout << "Машина осталась включенной!" << '\n';

        }
        break;
    }
    case 10:
        machine.reset();
        std::cout << "Машина успешно сброшена" << '\n';
        break;
        
    default:
        std::cout << "Нет такого пункта!" << '\n';
        break;
        
    case 0:
        return;
    }
}
}