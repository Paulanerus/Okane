#include "Option.hpp"

#include "impl/HelpOption.hpp"
#include "impl/DetailedOption.hpp"
#include "impl/StatusOption.hpp"
#include "impl/AddOption.hpp"
#include "impl/CurrencyOption.hpp"
#include "impl/RemoveOption.hpp"
#include "impl/AboOption.hpp"

const std::unordered_map<std::unordered_set<std::string>, std::shared_ptr<Option>, Option::HashUSet> Option::s_Args = {
    {{"help", "h"}, std::make_shared<HelpOption>()},
    {{"detailed", "det", "d"}, std::make_shared<DetailedOption>()},
    {{"status", "stat", "s"}, std::make_shared<StatusOption>()},
    {{"add", "a"}, std::make_shared<AddOption>()},
    {{"currency", "c"}, std::make_shared<CurrencyOption>()},
    {{"remove", "rem", "r"}, std::make_shared<RemoveOption>()},
    {{"abo"}, std::make_shared<AboOption>()},
};

std::shared_ptr<Option> Option::find(const std::string &arg)
{
    for (auto &[keys, val] : s_Args)
    {
        if (keys.find(arg) != keys.end())
            return val;
    }

    return nullptr;
}

std::vector<std::string> Option::copy_after(int argc, char **args)
{
    std::vector<std::string> argsLeft;

    for (int i = 2; i < argc; i++)
        argsLeft.emplace_back(args[i]);

    return argsLeft;
}