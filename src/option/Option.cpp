#include "Option.hpp"

#include "impl/HelpOption.hpp"
#include "impl/DetailedOption.hpp"
#include "impl/StatusOption.hpp"
#include "impl/AddOption.hpp"
#include "impl/CurrencyOption.hpp"

const std::vector<std::string> Option::args = {"help", "detailed", "status", "add", "currency"};

std::unique_ptr<Option> Option::find(const std::string &arg)
{
    if (std::find(args.begin(), args.end(), arg) == args.end())
    {
        return nullptr;
    }

    if (arg == "help")
    {
        return std::make_unique<HelpOption>();
    }
    else if (arg == "detailed")
    {
        return std::make_unique<DetailedOption>();
    }
    else if (arg == "status")
    {
        return std::make_unique<StatusOption>();
    }
    else if (arg == "add")
    {
        return std::make_unique<AddOption>();
    }
    else
    {
        return std::make_unique<CurrencyOption>();
    }
}

std::vector<std::string> Option::copyAfter(int argc, char **args)
{
    std::vector<std::string> argsLeft;

    for (int i = 2; i < argc; i++)
        argsLeft.emplace_back(args[i]);

    return argsLeft;
}

void Option::toLowerStr(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
}
