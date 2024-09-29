#pragma once

#include "rang.hpp"

#include "config.hpp"
#include "entry.hpp"
#include "option.hpp"
#include "regex.hpp"
#include "strings.hpp"
#include "table_view.hpp"
#include "time.hpp"

#include <iostream>
#include <numeric>

class StatusOption : public okane::Option {

public:
    const std::unordered_set<std::string> identifier() const noexcept
    {
        return std::unordered_set<std::string> { "status", "stat", "s" };
    }

    void execute(const std::vector<std::string>& args) override
    {
        std::string month = okane::time::to_string_fmt(okane::time::current_time(), "%m");
        std::string year = okane::time::to_string_fmt(okane::time::current_time(), "%Y");

        if (args.size() == 1) {
            auto month_by_id = okane::time::month_from_id(okane::strings::convert_to_lowercase(args[0]));

            if (!month_by_id.has_value()) {
                std::cout << rang::fgB::red << "Please provide a valid month. (January, Jan, 01 or 1)" << rang::style::reset << std::endl;
                return;
            }

            month = month_by_id.value();
        } else if (args.size() > 1) {
            auto month_by_id = okane::time::month_from_id(okane::strings::convert_to_lowercase(args[0]));

            if (!month_by_id.has_value()) {
                std::cout << rang::fgB::red << "Please provide a valid month. (January, Jan, 01 or 1)" << rang::style::reset << std::endl;
                return;
            }

            month = month_by_id.value();

            auto year_arg = args[1];

            if (!okane::rgx::matches_pnumber(year_arg)) {
                std::cout << rang::fgB::red << "Please provide a valid year. (2022 or 2023)" << rang::style::reset << std::endl;
                return;
            }

            year = year_arg;
        }

        const auto month_entry = okane::entry::find_month_by_id(okane::app_config().years, month, year);

        if (!month_entry.has_value()) {
            std::cout << rang::fgB::yellow << "You don't have any entry for " << month << '.' << year << "." << rang::style::reset << std::endl;
            return;
        }

        auto balance = month_entry->get().balance();

        okane::TableView table_view;

        table_view.add_row({ "Category", "Amount" });
        table_view.add_row({ "Income", okane::strings::convert_to_str_with_style(okane::strings::convert_to_str(month_entry->get().income()) + " " + okane::app_config().currency, rang::fgB::green) });
        table_view.add_row({ "Abos", okane::strings::convert_to_str_with_style(okane::strings::convert_to_str(month_entry->get().abos()) + " " + okane::app_config().currency, rang::fgB::yellow) });
        table_view.add_row({ "Expenses", okane::strings::convert_to_str_with_style(okane::strings::convert_to_str(month_entry->get().expenses()) + " " + okane::app_config().currency, rang::fgB::red) });
        table_view.add_row({ "Balance", okane::strings::convert_to_str_with_style(okane::strings::convert_to_str(balance) + " " + okane::app_config().currency, balance < 0 ? rang::fgB::red : rang::fgB::green) });

        table_view.print();
    }
};
