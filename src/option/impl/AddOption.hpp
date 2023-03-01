#pragma once

#include "../Option.hpp"
#include "../../regex/RegexHelper.hpp"
#include "../../entry/Entry.hpp"
#include "../../time/Time.hpp"
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

        std::string day = Okane::toStringFMT(Okane::getCurrentTime(), "%d");
        std::string month = Okane::toStringFMT(Okane::getCurrentTime(), "%m");
        std::string year = Okane::toStringFMT(Okane::getCurrentTime(), "%Y");

        if (!Okane::matchesAmount(args.at(0)))
        {
            std::cout << "Please enter a valid amount number (100, -6.6 or 12.35)";
            return;
        }

        if (!Okane::matchesTag(args.at(1)))
        {
            std::cout << "Please enter a valid tag";
            return;
        }

        amount = std::stod(args.at(0));
        tag = args.at(1);

        if (args.size() > 2)
        {
            if (!Okane::matchesDate(args.at(2)))
            {
                std::cout << "Please enter a valid date (01.01.2023, 1.1.2023, 1.01.2023, or 1.1.2023)";
                return;
            }

            std::vector<std::string> parts;
            std::stringstream stream(args.at(2));
            std::string tmp;

            while (std::getline(stream, tmp, '.'))
                parts.push_back(tmp);

            auto dayPart = parts.at(0);

            if (!Okane::matchesDay(dayPart))
            {
                std::cout << "Please provide a valid Day (1, 23 or 31)";
                return;
            }

            if (dayPart.length() == 1)
                dayPart = "0" + dayPart;

            day = dayPart;

            auto monthById = Okane::getMonthFromId(parts.at(1));

            if (!monthById.has_value())
            {
                std::cout << "Please provide a valid Month (01 or 1)";
                return;
            }

            month = monthById.value();

            auto yearArg = parts.at(2);

            if (!Okane::matchesYear(yearArg))
            {
                std::cout << "Please provide a valid Year (2022 or 2023)";
                return;
            }

            year = yearArg;
        }

        if (day == "29" && month == "02" && !Okane::isLeapYear(std::stol(year)))
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