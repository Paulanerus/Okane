#pragma once

#include "../Option.hpp"
#include "../../utils/OkaneUtils.hpp"
#include "../../entry/Entry.hpp"
#include "../../config/Config.hpp"

#include <iostream>

class AddOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        if (args.size() <= 1)
        {
            std::cout << "Please provide at least an amount and tag";
            return;
        }

        double amount{};
        std::string tag;

        std::string day = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%d");
        std::string month = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%m");
        std::string year = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%Y");

        if (!Okane::Regex::matchesAmount(args[0]))
        {
            std::cout << "Please enter a valid amount number (100, -6.6 or 12.35)";
            return;
        }

        if (!Okane::Regex::matchesTag(args[1]))
        {
            std::cout << "Please enter a valid tag";
            return;
        }

        amount = std::stod(args[0]);
        tag = args.at(1);

        if (args.size() > 2)
        {

            std::string date;
            if (!Okane::Time::getFormatDate(args[2], date))
            {
                std::cout << "Please enter a valid date (01.01.2023, 1.1.2023, 1.01.2023, or 1.1.2023)";
                return;
            }

            const auto parts = Okane::String::splitStr(date, '.');

            day = parts[0];
            month = parts[1];
            year = parts[2];
        }

        if (day == "29" && month == "02" && !Okane::Time::isLeapYear(std::stol(year)))
        {
            std::cout << "You've entered " << day << '.' << month << '.' << year << " Which is not a valid year (not a leap year)";
            return;
        }

        auto yearEntry = Entry::getYear(year);

        if (!yearEntry)
        {
            yearEntry = Entry::make_year(year);
            Config::appConfig.years.push_back(yearEntry);
        }

        auto monthEntry = Entry::getMonth(month, year);

        if (!monthEntry)
        {
            monthEntry = Entry::make_month(month);
            yearEntry->add(monthEntry);
        }

        monthEntry->add(Entry::make_simple((day + "." + month + "." + year), tag, amount));

        std::cout << "Successfully added Entry for " << day << '.' << month << '.' << year;
    }
};