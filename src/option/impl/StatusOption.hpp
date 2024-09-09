#pragma once

#include "rang.hpp"

#include "../../table/TableView.hpp"
#include "../../config/Config.hpp"
#include "../../utils/strings.hpp"
#include "../../entry/Entry.hpp"
#include "../../utils/regex.hpp"
#include "../../utils/time.hpp"
#include "../Option.hpp"

#include <numeric>
#include <iostream>

class StatusOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::string month = okane::time::to_string_fmt(okane::time::current_time(), "%m");
        std::string year = okane::time::to_string_fmt(okane::time::current_time(), "%Y");

        if (args.size() == 1)
        {
            auto month_by_id = okane::time::month_from_id(okane::strings::to_lower(args[0]));

            if (!month_by_id.has_value())
            {
                Okane::Logging::printlnError("Please provide a valid month. (January, Jan, 01 or 1)");
                return;
            }

            month = month_by_id.value();
        }
        else if (args.size() > 1)
        {
            auto month_by_id = okane::time::month_from_id(okane::strings::to_lower(args[0]));

            if (!month_by_id.has_value())
            {
                Okane::Logging::printlnError("Please provide a valid month. (January, Jan, 01 or 1)");
                return;
            }

            month = month_by_id.value();

            auto year_arg = args[1];

            if (!okane::rgx::matches_pnumber(year_arg))
            {
                Okane::Logging::printlnError("Please provide a valid year. (2022 or 2023)");
                return;
            }

            year = year_arg;
        }

        const auto month_entry = Entry::month(month, year);

        if (!month_entry)
        {
            Okane::Logging::printlnWarn("You don't have any entry for " + month + '.' + year + ".");
            return;
        }

        auto balance = month_entry->balance();

        TableView table_view;

        table_view.add_row({"Category", "Amount"});
        table_view.add_row({"Income", okane::strings::to_string_with_style(okane::strings::to_string(month_entry->income()) + " " + Config::s_AppConfig.currency, rang::fgB::green)});
        table_view.add_row({"Abos", okane::strings::to_string_with_style(okane::strings::to_string(month_entry->abos()) + " " + Config::s_AppConfig.currency, rang::fgB::yellow)});
        table_view.add_row({"Expenses", okane::strings::to_string_with_style(okane::strings::to_string(month_entry->expenses()) + " " + Config::s_AppConfig.currency, rang::fgB::red)});
        table_view.add_row({"Balance", okane::strings::to_string_with_style(okane::strings::to_string(balance) + " " + Config::s_AppConfig.currency, balance < 0 ? rang::fgB::red : rang::fgB::green)});

        table_view.print();
    }
};