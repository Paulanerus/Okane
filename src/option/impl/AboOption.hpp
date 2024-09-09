#pragma once

#include "../../table/TableView.hpp"
#include "../../config/Config.hpp"
#include "../../utils/strings.hpp"
#include "../../utils/regex.hpp"
#include "../../entry/Entry.hpp"
#include "../../utils/time.hpp"
#include "../Option.hpp"

#include <unordered_map>
#include <iostream>
#include <optional>
#include <memory>

class AboOption : public Option
{
public:
    void execute(const std::vector<std::string> &args)
    {
        if (args.empty())
        {
            TableView table_view;
            table_view.add_row({"Start date", "Tag", "Amount", "Interval"});

            for (const auto &abo : Config::s_AppConfig.abos)
                table_view.add_row({abo->date(), abo->tag(), okane::strings::to_string_with_style(okane::strings::to_string(abo->amount()) + " " + Config::s_AppConfig.currency, abo->amount() < 0 ? rang::fgB::red : rang::fgB::green), abo->interval() == PayInterval::YEARLY ? "Yearly" : "Monthly"});

            table_view.print();

            return;
        }

        if (args.size() < 2)
        {
            Okane::Logging::printlnError("Please provide at least an amount and tag.");
            return;
        }

        if (!okane::rgx::matches_amount(args[0]) || !okane::rgx::matches_tag(args[1]))
        {
            Okane::Logging::printlnError("Please provide a valid amount and tag.");
            return;
        }

        double amount = std::stod(args[0]);
        std::string tag = args[1];

        std::string date = okane::time::to_string_fmt(okane::time::current_time(), "%d.%m.%Y");

        auto interval = PayInterval::MONTHLY;

        if (args.size() == 3)
        {
            auto subscription = is_interval(args[2]);

            if (subscription.has_value())
                interval = subscription.value();
            else if (!okane::time::format_date(args[2], date))
            {
                Okane::Logging::printlnError("Please enter a valid interval or date.");
                return;
            }
        }

        if (args.size() > 3)
        {
            auto subscription = is_interval(args[2]);

            if (!subscription.has_value())
            {
                Okane::Logging::printlnError("Please provide a valid interval value. (0, monthly, month or 1, yearly, year)");
                return;
            }

            interval = subscription.value();

            if (!okane::time::format_date(args[3], date))
            {
                Okane::Logging::printlnError("Please enter a valid date. (01.01.2023, 1.1.2023, 1.01.2023, or 1.1.2023)");
                return;
            }
        }

        auto duplicate = std::find_if(Config::s_AppConfig.abos.begin(), Config::s_AppConfig.abos.end(), [tag, amount](const shared_abo &_abo)
                                      { return _abo->tag() == tag && _abo->amount() == amount; });

        if (duplicate != Config::s_AppConfig.abos.end())
        {
            Okane::Logging::printlnWarn("There is already a similar abo. (" + tag + ", " + okane::strings::to_string(amount) + ")");
            return;
        }

        Config::s_AppConfig.abos.push_back(Entry::make_abo(date, tag, amount, interval));

        Okane::Logging::println("Successfully added abo!");
    }

private:
    const std::unordered_map<PayInterval, std::vector<std::string>> INTERVALS = {
        {PayInterval::MONTHLY, {"0", "monthly", "month"}},
        {PayInterval::YEARLY, {"1", "01", "yearly", "year"}}};

    std::optional<PayInterval> is_interval(const std::string &input)
    {
        auto lowered_input = okane::strings::to_lower(input);

        for (const auto &[interval, ids] : INTERVALS)
        {
            if (std::find(ids.begin(), ids.end(), lowered_input) != ids.end())
                return interval;
        }

        return {};
    }
};
