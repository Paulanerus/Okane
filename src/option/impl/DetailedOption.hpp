#pragma once

#include "../../table/TableView.hpp"
#include "../../config/Config.hpp"
#include "../../utils/strings.hpp"
#include "../../utils/regex.hpp"
#include "../../entry/Entry.hpp"
#include "../../utils/time.hpp"
#include "../Option.hpp"

#include <iostream>
#include <cstdint>

class DetailedOption : public Option
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
                std::cout << rang::fgB::red << "Please provide a valid month. (January, Jan, 01 or 1)" << rang::style::reset << std::endl;
                return;
            }

            month = month_by_id.value();
        }
        else if (args.size() > 1)
        {
            auto month_by_id = okane::time::month_from_id(okane::strings::to_lower(args[0]));

            if (!month_by_id.has_value())
            {
                std::cout << rang::fgB::red << "Please provide a valid month. (January, Jan, 01 or 1)" << rang::style::reset << std::endl;
                return;
            }

            month = month_by_id.value();

            auto year_arg = args[1];

            if (!okane::rgx::matches_pnumber(year_arg))
            {
                std::cout << rang::fgB::red << "Please provide a valid year. (2022 or 2023)" << rang::style::reset << std::endl;
                return;
            }

            year = year_arg;
        }

        const auto month_entry = Entry::month(month, year);

        if (!month_entry)
        {
            std::cout << rang::fgB::yellow << "You don't have any entry for " << month << '.' << year << "." << rang::style::reset << std::endl;
            return;
        }

        TableView table_view;
        table_view.add_row({"Index", "Date", "Tag", "Amount"});

        for (std::size_t i{}; i < month_entry->entries.size(); i++)
        {
            auto amount = month_entry->entries[i]->amount();

            if (month_entry->entries[i]->type() == EntryType::ABO && std::static_pointer_cast<AboEntry>(month_entry->entries[i])->interval() == PayInterval::YEARLY)
                amount /= 12;

            table_view.add_row(
                {std::to_string(i), month_entry->entries[i]->date(), month_entry->entries[i]->tag(),
                 okane::strings::to_string_with_style(okane::strings::to_string(amount) + " " + Config::s_AppConfig.currency, month_entry->entries[i]->type() == EntryType::ABO ? rang::fgB::yellow : amount < 0 ? rang::fgB::red
                                                                                                                                                                                                                 : rang::fgB::green)});
        }

        table_view.print();
    }
};