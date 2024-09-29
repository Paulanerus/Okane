#include "rang.hpp"

#include "config.hpp"
#include "option.hpp"
#include "strings.hpp"

#include <clocale>
#include <iostream>

#ifdef _WIN32
#    include "Windows.h"
#endif

int main(int argc, char** args)
{
    rang::setControlMode(rang::control::Force);

    if (argc <= 1) {
        std::cout << "Try '" << rang::fgB::blue << "okane help" << rang::style::reset << "' for more information" << std::endl;
        return EXIT_SUCCESS;
    }

#ifdef _WIN32
    rang::setWinTermMode(rang::winTerm::Ansi);

    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_CTYPE, "");
#else
    setlocale(LC_CTYPE, "en_US.utf8");
#endif

    okane::load_config();

    std::string firstArg { args[1] };
    auto option = okane::find_by_name(okane::strings::convert_to_lowercase(firstArg));

    if (!option) {
        std::cout << rang::fgB::red << "\nOption not found try 'okane help' for more information\n"
                  << rang::style::reset << std::endl;
        return EXIT_SUCCESS;
    }

    std::cout << std::endl;
    option->execute(okane::copy_after(argc, args));
    std::cout << std::endl;

    okane::save_config();

    return EXIT_SUCCESS;
}
