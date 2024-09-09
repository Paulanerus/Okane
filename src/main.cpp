#include "rang.hpp"

#include "config/Config.hpp"
#include "option/Option.hpp"
#include "utils/strings.hpp"

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
    auto option = Option::find(okane::strings::to_lower(firstArg));

    if (!option)
    {
        std::cout << rang::fgB::red << "\nOption not found try 'okane help' for more informations\n"
                  << rang::style::reset << std::endl;
        return EXIT_SUCCESS;
    }

    std::cout << std::endl;
    option->execute(Option::copy_after(argc, args));
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
