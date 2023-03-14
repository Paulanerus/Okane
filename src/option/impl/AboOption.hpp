#pragma once

#include "../Option.hpp"
#include "../../entry/Entry.hpp"
#include "../../config/Config.hpp"
#include "../../utils/OkaneUtils.hpp"
#include "../../table/TableView.hpp"

#include <iostream>
#include <unordered_map>
#include <optional>
#include <memory>

class AboOption : public Option
{
public:
    void execute(const std::vector<std::string> &args)
    {
        if (args.size() == 0)
        {
            auto tableView = std::make_unique<TableView>();
            tableView->addRow({"Start date", "Tag", "Amount", "Interval"});

            for (const auto &abo : Config::appConfig.abos)
                tableView->addRow({abo->getDate(), abo->getTag(), Okane::String::toStringWithStyle(Okane::String::toString(abo->getAmount()) + " " + Config::appConfig.currency, abo->getAmount() < 0 ? rang::fgB::red : rang::fgB::green), abo->getInterval() == 1 ? "Yearly" : "Monthly"});

            tableView->print();

            return;
        }

        if (args.size() < 2)
        {
            Okane::Logging::printlnError("Please provide at least an amount and tag.");
            return;
        }

        if (!Okane::Regex::matchesAmount(args[0]) || !Okane::Regex::matchesTag(args[1]))
        {
            Okane::Logging::printlnError("Please provide a valid amount and tag.");
            return;
        }

        double amount = std::stod(args[0]);
        std::string tag = args[1];

        std::string date = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%d.%m.%Y");

        auto interval = PayInterval::MONTHLY;

        if (args.size() == 3)
        {
            auto subscription = isInterval(args[2]);

            if (subscription.has_value())
                interval = subscription.value();
            else if (!Okane::Time::getFormatDate(args[2], date))
            {
                Okane::Logging::printlnError("Please enter a valid interval or date.");
                return;
            }
        }

        if (args.size() > 3)
        {
            auto subscription = isInterval(args[2]);

            if (!subscription.has_value())
            {
                Okane::Logging::printlnError("Please provide a valid interval value. (0, monthly, month or 1, yearly, year)");
                return;
            }

            interval = subscription.value();

            if (!Okane::Time::getFormatDate(args[3], date))
            {
                Okane::Logging::printlnError("Please enter a valid date. (01.01.2023, 1.1.2023, 1.01.2023, or 1.1.2023)");
                return;
            }
        }

        auto duplicate = std::find_if(Config::appConfig.abos.begin(), Config::appConfig.abos.end(), [tag, amount](const shared_abo &_abo)
                                      { return _abo->getTag() == tag && _abo->getAmount() == amount; });

        if (duplicate != Config::appConfig.abos.end())
        {
            Okane::Logging::printlnWarn("There is already a similar abo. (" + tag + ", " + Okane::String::toString(amount) + ")");
            return;
        }

        Config::appConfig.abos.push_back(Entry::make_abo(date, tag, amount, interval));

        Okane::Logging::println("Successfully added abo!");
    }

private:
    const std::unordered_map<PayInterval, std::vector<std::string>> INTERVALS = {
        {PayInterval::MONTHLY, {"0", "monthly", "month"}},
        {PayInterval::YEARLY, {"1", "01", "yearly", "year"}}};

    std::optional<PayInterval> isInterval(const std::string &input)
    {
        auto lowerInput = Okane::String::toLower(input);

        for (const auto &[interval, ids] : INTERVALS)
        {
            if (std::find(ids.begin(), ids.end(), lowerInput) != ids.end())
                return interval;
        }

        return {};
    }
};
