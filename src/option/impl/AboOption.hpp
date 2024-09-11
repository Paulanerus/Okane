#pragma once

#include "../../table/TableView.hpp"
#include "../Option.hpp"
#include "strings.hpp"
#include "config.hpp"
#include "entry.hpp"
#include "regex.hpp"
#include "time.hpp"

#include <unordered_map>
#include <iostream>
#include <optional>
#include <memory>

class AboOption : public Option
{
public:
    void execute(const std::vector<std::string> &args)
    {
        auto &abos = okane::app_config().abos;

        if (args.empty())
        {
            TableView table_view;
            table_view.add_row({"Start date", "Tag", "Amount", "Interval"});

            for (const auto &abo : abos)
                table_view.add_row({abo.date, abo.tag, okane::strings::convert_to_str_with_style(okane::strings::convert_to_str(abo.amount) + " " + okane::app_config().currency, abo.amount < 0 ? rang::fgB::red : rang::fgB::green), abo.interval == okane::entry::PayInterval::Yearly ? "Yearly" : "Monthly"});

            table_view.print();
            return;
        }

        if (args.size() < 2)
        {
            std::cout << rang::fgB::red << "Please provide at least an amount and tag." << rang::style::reset << std::endl;
            return;
        }

        if (!okane::rgx::matches_amount(args[0]) || !okane::rgx::matches_tag(args[1]))
        {
            std::cout << rang::fgB::red << "Please provide a valid amount and tag." << rang::style::reset << std::endl;
            return;
        }

        double amount = std::stod(args[0]);
        std::string tag = args[1];

        std::string date = okane::time::to_string_fmt(okane::time::current_time(), "%d.%m.%Y");

        auto interval = okane::entry::PayInterval::Monthly;

        if (args.size() == 3)
        {
            auto subscription = is_interval(args[2]);

            if (subscription.has_value())
                interval = subscription.value();
            else if (!okane::time::format_date(args[2], date))
            {
                std::cout << rang::fgB::red << "Please enter a valid interval or date." << rang::style::reset << std::endl;
                return;
            }
        }

        if (args.size() > 3)
        {
            auto subscription = is_interval(args[2]);

            if (!subscription.has_value())
            {
                std::cout << rang::fgB::red << "Please provide a valid interval value. (0, monthly, month or 1, yearly, year)" << rang::style::reset << std::endl;
                return;
            }

            interval = subscription.value();

            if (!okane::time::format_date(args[3], date))
            {
                std::cout << rang::fgB::red << "Please enter a valid date. (01.01.2023, 1.1.2023, 1.01.2023, or 1.1.2023)" << rang::style::reset << std::endl;
                return;
            }
        }

        auto duplicate = std::find_if(abos.begin(), abos.end(), [tag, amount](const okane::entry::Entry &_abo)
                                      { return _abo.tag == tag && _abo.amount == amount; });

        if (duplicate != abos.end())
        {
            std::cout << rang::fgB::yellow << "There is already a similar abo. (" << tag << ", " << okane::strings::convert_to_str(amount) << ")" << rang::style::reset << std::endl;
            return;
        }

        abos.push_back(okane::entry::make_abo(amount, tag, date, interval));

        std::cout << rang::fgB::green << "Successfully added abo!" << rang::style::reset << std::endl;
    }

private:
    const std::unordered_map<okane::entry::PayInterval, std::vector<std::string>> INTERVALS = {
        {okane::entry::PayInterval::Monthly, {"0", "monthly", "month"}},
        {okane::entry::PayInterval::Yearly, {"1", "01", "yearly", "year"}}};

    std::optional<okane::entry::PayInterval> is_interval(const std::string &input)
    {
        auto lowered_input = okane::strings::convert_to_lowercase(input);

        for (const auto &[interval, ids] : INTERVALS)
        {
            if (std::find(ids.begin(), ids.end(), lowered_input) != ids.end())
                return interval;
        }

        return {};
    }
};
