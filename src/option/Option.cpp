#include "Option.hpp"

#include <algorithm>

#include "impl/HelpOption.hpp"
#include "impl/DetailedOption.hpp"
#include "impl/StatusOption.hpp"
#include "impl/AddOption.hpp"
#include "impl/CurrencyOption.hpp"

const std::unordered_map<std::string, std::shared_ptr<Option>> Option::m_Args = {
    {"help", std::make_shared<HelpOption>()},
    {"detailed", std::make_shared<DetailedOption>()},
    {"status", std::make_shared<StatusOption>()},
    {"add", std::make_shared<AddOption>()},
    {"currency", std::make_shared<CurrencyOption>()}};

std::shared_ptr<Option> Option::find(const std::string &arg)
{
    auto iter = m_Args.find(arg);

    return iter == m_Args.end() ? nullptr : iter->second;
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
