#pragma once

#include "../Option.hpp"
#include "../../utils/RegexUtils.hpp"
#include "../../utils/TimeUtils.hpp"
#include "../../entry/Entry.hpp"
#include <iostream>

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

        if (!Okane::Regex::matchesIndex(args.at(0)))
        {
            std::cout << "Please enter a valid index (0, 1 or 20)";
            return;
        }

        size_t index = std::stoul(args.at(0));

        std::string month = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%m");
        std::string year = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%Y");

        if (args.size() >= 2)
        {
            auto monthById = Okane::Time::getMonthFromId(args.at(1));

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

            auto yearArg = args.at(2);

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

        if (!monthEntry->erase(index))
        {
            std::cout << "Your provided index (" << index << ") is greater than the amount of entries for " << month << '.' << year;
            return;
        }

        std::cout << "Successfully removed Entry";
    }
};