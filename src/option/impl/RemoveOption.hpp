#pragma once

#include "../Option.hpp"
#include "../../utils/RegexUtils.hpp"
#include "../../utils/TimeUtils.hpp"
#include "../../entry/Entry.hpp"
#include "../../config/Config.hpp"
#include <iostream>
#include <unordered_set>

class RemoveOption : public Option
{
public:
    void execute(const std::vector<std::string> &args) override
    {
        if (args.size() < 1)
        {
            Okane::Logging::printlnError("Please provide at least an index.");
            return;
        }

        if (!Okane::Regex::matchesPNumber(args[0]))
        {
            Okane::Logging::printlnError("Please enter a valid index. (0, 1 or 20)");
            return;
        }

        size_t index = std::stoul(args[0]);

        std::string month = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%m");
        std::string year = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%Y");

        if (args.size() >= 2)
        {
            auto monthById = Okane::Time::getMonthFromId(Okane::String::toLower(args[1]));

            if (!monthById.has_value())
            {
                Okane::Logging::printlnError("Please provide a valid month. (January, Jan, 01 or 1)");
                return;
            }

            month = monthById.value();

            if (args.size() == 2)
            {
                return;
            }

            auto yearArg = args[2];

            if (!Okane::Regex::matchesPNumber(yearArg))
            {
                Okane::Logging::printlnError("Please provide a valid year. (2022 or 2023)");
                return;
            }

            year = yearArg;
        }

        auto monthEntry = Entry::getMonth(month, year);

        if (!monthEntry)
        {
            Okane::Logging::printlnWarn("You don't have any entries for " + month + '.' + year + ".");
            return;
        }

        if (index < monthEntry->entries.size() && monthEntry->entries[index]->getType() == EntryType::ABO)
        {
            Okane::Logging::printlnWarn("The provided index belongs to an abo entry. Do you want to completely delete the abo? (Y/n)");

            char input = std::getchar();

            if (ALLOWED_YES.find(input) == ALLOWED_YES.end())
            {
                Okane::Logging::println("Aborted...");
                return;
            }

            auto castPtr = std::static_pointer_cast<AboEntry>(monthEntry->entries[index]);

            auto aboEntry = std::find_if(Config::appConfig.abos.begin(), Config::appConfig.abos.end(), [castPtr](const shared_abo &abo)
                                         { return abo->getTag() == castPtr->getTag() && abo->getAmount() == castPtr->getAmount(); });

            if (aboEntry == Config::appConfig.abos.end())
            {
                Okane::Logging::printlnError("Surprise! The entry isn't there anymore... (You should not see this)");
                return;
            }

            Config::appConfig.abos.erase(aboEntry);

            Okane::Logging::println("Successfully removed abo!");
            return;
        }

        if (!monthEntry->erase(index))
        {
            Okane::Logging::printlnError("Your provided index (" + std::to_string(index) + ") is greater than the amount of entries for " + month + '.' + year + ".");
            return;
        }

        Okane::Logging::println("Successfully removed entry!");
    }

private:
    const std::unordered_set<char> ALLOWED_YES = {'y', 'Y', '\n'};
};