#pragma once

#include "rang.hpp"

#include "../Option.hpp"
#include "../../utils/OkaneUtils.hpp"
#include "../../entry/Entry.hpp"
#include "../../table/TableView.hpp"
#include "../../config/Config.hpp"

#include <numeric>
#include <iostream>

class StatusOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::string month = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%m");
        std::string year = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%Y");

        if (args.size() == 1)
        {
            auto monthById = Okane::Time::getMonthFromId(args[0]);

            if (!monthById.has_value())
            {
                Okane::Logging::printlnError("Please provide a valid month. (January, Jan, 01 or 1)");
                return;
            }

            month = monthById.value();
        }
        else if (args.size() > 1)
        {
            auto monthById = Okane::Time::getMonthFromId(args[0]);

            if (!monthById.has_value())
            {
                Okane::Logging::printlnError("Please provide a valid month. (January, Jan, 01 or 1)");
                return;
            }

            month = monthById.value();

            auto yearArg = args[1];

            if (!Okane::Regex::matchesYear(yearArg))
            {
                Okane::Logging::printlnError("Please provide a valid year. (2022 or 2023)");
                return;
            }

            year = yearArg;
        }

        const auto monthEntry = Entry::getMonth(month, year);

        if (!monthEntry)
        {
            Okane::Logging::printlnWarn("You don't have any entry for " + month + '.' + year + ".");
            return;
        }

        auto balance = monthEntry->getBalance();

        auto table = std::make_unique<TableView>();

        table->addRow({"Category", "Amount"});
        table->addRow({"Income", Okane::String::toStringWithStyle(Okane::String::toString(monthEntry->getIncome()) + " " + Config::appConfig.currency, rang::fgB::green)});
        table->addRow({"Abos", Okane::String::toStringWithStyle(Okane::String::toString(monthEntry->getAbos()) + " " + Config::appConfig.currency, rang::fgB::yellow)});
        table->addRow({"Expenses", Okane::String::toStringWithStyle(Okane::String::toString(monthEntry->getExpenses()) + " " + Config::appConfig.currency, rang::fgB::red)});
        table->addRow({"Balance", Okane::String::toStringWithStyle(Okane::String::toString(balance) + " " + Config::appConfig.currency, balance < 0 ? rang::fgB::red : rang::fgB::green)});

        table->print();
    }
};