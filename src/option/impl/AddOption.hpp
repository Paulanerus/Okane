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
            if (!Okane::Regex::matchesDate(args[2]))
            {
                std::cout << "Please enter a valid date (01.01.2023, 1.1.2023, 1.01.2023, or 1.1.2023)";
                return;
            }

            auto parts = Okane::String::split_str(args[2], '.');

            auto dayPart = parts[0];

            if (!Okane::Regex::matchesDay(dayPart))
            {
                std::cout << "Please provide a valid Day (1, 23 or 31)";
                return;
            }

            if (dayPart.length() == 1)
                dayPart = "0" + dayPart;

            day = dayPart;

            auto monthById = Okane::Time::getMonthFromId(parts[1]);

            if (!monthById.has_value())
            {
                std::cout << "Please provide a valid Month (01 or 1)";
                return;
            }

            month = monthById.value();

            auto yearArg = parts[2];

            if (!Okane::Regex::matchesYear(yearArg))
            {
                std::cout << "Please provide a valid Year (2022 or 2023)";
                return;
            }

            year = yearArg;
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