#include "rang.hpp"

#include "config/Config.hpp"
#include "option/Option.hpp"
#include "utils/StringUtils.hpp"

#include <clocale>
#include <iostream>

#ifdef _WIN32
#include "Windows.h"
#endif
int main(int argc, char **args)
{
    if (argc <= 1)
        return EXIT_SUCCESS;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_CTYPE, "");
#else
    setlocale(LC_CTYPE, "en_US.utf8");
#endif

    rang::setControlMode(rang::control::Force);

    auto config = std::make_unique<Config>();

    std::string firstArg{args[1]};
    Okane::String::toLower(firstArg);

    auto option = Option::find(firstArg);

    if (!option)
    {
        std::cout << rang::fg::red << "Option not found try 'okane help' for more informations" << rang::style::reset << std::endl;
        return EXIT_SUCCESS;
    }

    std::vector<std::string> restOfArgs = Option::copyAfter(argc, args);
    std::cout << std::endl;
    option->execute(restOfArgs);
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
