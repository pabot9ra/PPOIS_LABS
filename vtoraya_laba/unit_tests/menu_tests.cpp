#include <UnitTest++/UnitTest++.h>
#include "menu.h"
#include "demos.h"
#include <sstream>
#include <stdexcept>
#include <string>

TEST(MenuExitsOnZeroAndEndOfInput) {
    std::istringstream exitInput("0\n");
    std::ostringstream output;
    CHECK_EQUAL(0, runMenu(exitInput, output));
    std::istringstream emptyInput;
    CHECK_EQUAL(0, runMenu(emptyInput, output));
}

TEST(MenuRecoversFromInvalidChoiceAndIntegerInput) {
    std::istringstream input("abc\n1 trailing\n99\n0\n");
    std::ostringstream output;
    CHECK_EQUAL(0, runMenu(input, output));
    CHECK(output.str().find("Введите целое число") != std::string::npos);
    CHECK(output.str().find("После числа") != std::string::npos);
    CHECK(output.str().find("Нет такого пункта") != std::string::npos);
}

TEST(MenuShowsEmptyNetworkSummary) {
    std::istringstream input("8\n0\n");
    std::ostringstream output;
    CHECK_EQUAL(0, runMenu(input, output));
    CHECK(output.str().find("Клиентов: 0") != std::string::npos);
    CHECK(output.str().find("Отправлений: 0") != std::string::npos);
}

TEST(MenuHandlesInterruptedCustomerInput) {
    std::istringstream input("1\n7\n");
    std::ostringstream output;
    CHECK_EQUAL(0, runMenu(input, output));
    CHECK(output.str().find("Клиент зарегистрирован") == std::string::npos);
}

TEST(DemonstrationsRunAllPostalWorkflows) {
    std::ostringstream output;
    runDemonstrations(output);
    CHECK(output.str().find("ОПЛАТА И ВОЗВРАТ") != std::string::npos);
    CHECK(output.str().find("ХРАНЕНИЕ И ПЕРЕВОЗКА") != std::string::npos);
    CHECK(output.str().find("ОЧЕРЕДЬ И СОТРУДНИКИ") != std::string::npos);
    CHECK_THROW(runDemonstration(0, output), std::invalid_argument);
    CHECK_THROW(runDemonstration(6, output), std::invalid_argument);
}
