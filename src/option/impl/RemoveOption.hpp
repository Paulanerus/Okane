#pragma once

#include "../Option.hpp"
#include "../../utils/RegexUtils.hpp"
#include "../../utils/TimeUtils.hpp"
#include "../../entry/Entry.hpp"
#include <iostream>
#include <unordered_set>

class RemoveOption : public Option
{
public:
    void execute(const std::vector<std::string> &args) override
    {
        if (args.size() < 1)
        {
            std::cout << "Please provide at least an index";
            return;
        }

        if (!Okane::Regex::matchesIndex(args[0]))
        {
            std::cout << "Please enter a valid index (0, 1 or 20)";
            return;
        }

        size_t index = std::stoul(args[0]);

        std::string month = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%m");
        std::string year = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%Y");

        if (args.size() >= 2)
        {
            auto monthById = Okane::Time::getMonthFromId(args[1]);

            if (!monthById.has_value())
            {
                std::cout << "Please provide a valid Month (January, Jan, 01 or 1)" << std::endl;
                return;
            }

            month = monthById.value();

            if (args.size() == 2)
            {
                return;
            }

            auto yearArg = args[2];

            if (!Okane::Regex::matchesYear(yearArg))
            {
                std::cout << "Please provide a valid Year (2022 or 2023)" << std::endl;
                return;
            }

            year = yearArg;
        }

        auto monthEntry = Entry::getMonth(month, year);

        if (!monthEntry)
        {
            std::cout << "You don't have any entries for " << month << '.' << year;
            return;
        }

        if (index >= 0 && index < monthEntry->entries.size() && monthEntry->entries[index]->getType() == EntryType::ABO)
        {
            std::cout
                << "The provided index belongs to an abo entry. Do want to completely delete the abo? (Y/n)" << std::endl;

            char input = std::getchar();

            if (ALLOWED_YES.find(input) == ALLOWED_YES.end())
            {
                std::cout << "Aborted...";
                return;
            }

            auto castPtr = std::static_pointer_cast<AboEntry>(monthEntry->entries[index]);

            auto aboEntry = std::find_if(Config::appConfig.abos.begin(), Config::appConfig.abos.end(), [castPtr](const shared_abo &abo)
                                         { return abo->getTag() == castPtr->getTag() && abo->getAmount() == castPtr->getAmount(); });

            if (aboEntry == Config::appConfig.abos.end())
            {
                std::cout << "Surprise! The entry isn't there anymore... (You should not see this)";
                return;
            }

            Config::appConfig.abos.erase(aboEntry);

            std::cout << "Successfully removed Abo";
            return;
        }

        if (!monthEntry->erase(index))
        {
            std::cout << "Your provided index (" << index << ") is greater than the amount of entries for " << month << '.' << year;
            return;
        }

        std::cout << "Successfully removed Entry";
    }

private:
    const std::unordered_set<char> ALLOWED_YES = {'y', 'Y', '\n'};
};