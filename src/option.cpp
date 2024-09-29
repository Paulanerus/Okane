#include "option.hpp"

#include "impl/AboOption.hpp"
#include "impl/AddOption.hpp"
#include "impl/CurrencyOption.hpp"
#include "impl/DetailedOption.hpp"
#include "impl/HelpOption.hpp"
#include "impl/RemoveOption.hpp"
#include "impl/StatusOption.hpp"

namespace okane {
std::shared_ptr<Option> find_by_name(const std::string& arg) noexcept
{
    static const std::unordered_set<std::shared_ptr<Option>> OPTIONS = {
        { std::make_shared<DetailedOption>() },
        { std::make_shared<CurrencyOption>() },
        { std::make_shared<RemoveOption>() },
        { std::make_shared<StatusOption>() },
        { std::make_shared<HelpOption>() },
        { std::make_shared<AddOption>() },
        { std::make_shared<AboOption>() }
    };

    for (auto& option : OPTIONS) {
        for (auto& id : option->identifier()) {
            if (id == arg)
                return option;
        }
    }

    return nullptr;
}

std::vector<std::string> copy_after(int argc, char** argv) noexcept
{
    std::vector<std::string> args { argv + 2, argv + argc };

    return args;
}
}
