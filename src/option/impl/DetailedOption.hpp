#pragma once

#include "../Option.hpp"
#include "../../time/Time.hpp"
#include "../../entry/Entry.hpp"
#include "../../table/TableView.hpp"
#include "../../regex/RegexHelper.hpp"

#include <iostream>

class DetailedOption : public Option
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

            year = yearArg;
        }

        const auto monthEntry = Okane::getMonth(month, year);

        if (!monthEntry.has_value())
        {
            std::cout << "You don't have any entry for " << month << '.' << year << std::endl;
            return;
        }

        auto monthValue = monthEntry.value();

        TableView tableView;
        tableView << std::vector<std::string>{"Date", "Tag", "Amount"};

        for (const auto &entry : monthValue.entries)
            tableView << std::vector<std::string>{entry.date, entry.tag, TableView::to_string(entry.amount)};

        tableView.print();
    }
};