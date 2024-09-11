#pragma once

#include "../../utils/regex.hpp"
#include "../../utils/time.hpp"
#include "../Option.hpp"
#include "strings.hpp"
#include "config.hpp"
#include "entry.hpp"

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

        auto year_entry = okane::entry::find_year_by_id(okane::app_config().years, year);

        if (!year_entry.has_value())
            okane::app_config().years.push_back(okane::entry::make_year(year));

        auto month_entry = okane::entry::find_month_by_id(okane::app_config().years, month, year);

        if (!month_entry.has_value())
            (year_entry.has_value() ? year_entry->get() : okane::app_config().years.back()).months.push_back(okane::entry::make_month(month));

        (month_entry.has_value() ? month_entry->get() : (year_entry.has_value() ? year_entry->get() : okane::app_config().years.back()).months.back()).add(okane::entry::make_entry(amount, tag, (day + "." + month + "." + year)));

        std::cout << rang::fgB::green << "Successfully added entry!" << rang::style::reset << std::endl;
    }
};