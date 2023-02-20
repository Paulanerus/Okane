#include "config/Config.hpp"
#include "option/Option.hpp"

int main(int argc, char **args)
{
    if (argc <= 1)
    {
        return EXIT_SUCCESS;
    }

    rang::setControlMode(rang::control::Force);

    auto config = std::make_unique<Config>();

    std::string firstArg{args[1]};
    Option::toLowerStr(firstArg);

    auto option = Option::find(firstArg);

    if (!option)
    {
        std::cout << rang::fg::red << "Option not found try 'okane help' for more informations" << rang::style::reset << std::endl;
        return EXIT_SUCCESS;
    }

    std::vector<std::string> restOfArgs = Option::copyAfter(argc, args);
    option->execute(restOfArgs);

    return EXIT_SUCCESS;
}
