#pragma once

#include "../../config/Config.hpp"
#include "../../utils/strings.hpp"
#include "../../entry/Entry.hpp"
#include "../../utils/regex.hpp"
#include "../../utils/time.hpp"
#include "../Option.hpp"

#include <iostream>

class AddOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        if (args.size() <= 1)
        {
            std::cout << rang::fgB::red << "Please provide at least an amount and tag." << rang::style::reset << std::endl;
            return;
        }

        double amount{};
        std::string tag;

        std::string day = okane::time::to_string_fmt(okane::time::current_time(), "%d");
        std::string month = okane::time::to_string_fmt(okane::time::current_time(), "%m");
        std::string year = okane::time::to_string_fmt(okane::time::current_time(), "%Y");

        if (!okane::rgx::matches_amount(args[0]))
        {
            std::cout << rang::fgB::red << "Please enter a valid amount number. (100, -6.6 or 12.35)" << rang::style::reset << std::endl;
            return;
        }

        if (!okane::rgx::matches_tag(args[1]))
        {
            std::cout << rang::fgB::red << "Please enter a valid tag." << rang::style::reset << std::endl;
            return;
        }

        amount = std::stod(args[0]);
        tag = args[1];

        if (args.size() > 2)
        {

            std::string date;
            if (!okane::time::format_date(args[2], date))
            {
                std::cout << rang::fgB::red << "Please enter a valid date. (01.01.2023, 1.1.2023, 1.01.2023, or 1.1.2023)" << rang::style::reset << std::endl;
                return;
            }

            const auto parts = okane::strings::split_str(date, '.');

            day = parts[0];
            month = parts[1];
            year = parts[2];
        }

        if (day == "29" && month == "02" && !okane::time::is_leap_year(std::stol(year)))
        {
            std::cout << rang::fgB::red << "You've entered " << day << '.' << month << '.' << year << " which is not a valid year. (not a leap year)" << rang::style::reset << std::endl;
            return;
        }

        auto year_entry = Entry::year(year);

        if (!year_entry)
        {
            year_entry = Entry::make_year(year);
            Config::s_AppConfig.years.push_back(year_entry);
        }

        auto month_entry = Entry::month(month, year);

        if (!month_entry)
        {
            month_entry = Entry::make_month(month);
            year_entry->add(month_entry);
        }

        month_entry->add(Entry::make_simple((day + "." + month + "." + year), tag, amount));

        std::cout << rang::fgB::green << "Successfully added entry!" << rang::style::reset << std::endl;
    }
};