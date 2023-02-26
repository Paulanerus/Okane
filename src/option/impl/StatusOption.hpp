#pragma once

#include "rang.hpp"

#include "../Option.hpp"
#include "../../time/Time.hpp"
#include "../../entry/Entry.hpp"
#include "../../table/TableView.hpp"
#include "../../regex/RegexHelper.hpp"

#include <iostream>

class StatusOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::string month = Okane::toStringFMT(Okane::getCurrentTime(), "%m");
        std::string year = Okane::toStringFMT(Okane::getCurrentTime(), "%Y");

        if (args.size() == 1)
        {
            auto monthById = Okane::getMonthFromId(args.at(0));

            if (!monthById.has_value())
            {
                std::cout << "Please provide a valid Month (January, Jan, 01 or 1)" << std::endl;
                return;
            }

            month = monthById.value();
        }
        else if (args.size() > 1)
        {
            auto monthById = Okane::getMonthFromId(args.at(0));

            if (!monthById.has_value())
            {
                std::cout << "Please provide a valid Month (January, Jan, 01 or 1)" << std::endl;
                return;
            }

            month = monthById.value();

            auto yearArg = args.at(1);

            if (!Okane::matchesYear(yearArg))
            {
                std::cout << "Please provide a valid Year (2022 or 2023)" << std::endl;
                return;
            }
        }

        const auto monthEntry = Okane::getMonth(month, year);

        if (!monthEntry.has_value())
        {
            std::cout << "You don't have any entry for " << month << '.' << year << std::endl;
            return;
        }

        auto monthValue = monthEntry.value();

        TableView tableView;

        tableView << "Balance";
        tableView << TableView::to_string(monthValue.getIncome());
        tableView << TableView::to_string(monthValue.getExpenses());
        tableView << TableView::to_string(monthValue.getBalance());

        tableView.print();
    }
};