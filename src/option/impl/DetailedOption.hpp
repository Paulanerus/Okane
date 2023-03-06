#pragma once

#include "../Option.hpp"
#include "../../time/Time.hpp"
#include "../../entry/Entry.hpp"
#include "../../table/TableView.hpp"
#include "../../regex/RegexHelper.hpp"
#include "../../config/Config.hpp"

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

        const auto monthEntry = Entry::getMonth(month, year);

        if (!monthEntry)
        {
            std::cout << "You don't have any entry for " << month << '.' << year << std::endl;
            return;
        }

        std::sort(monthEntry->entries.begin(), monthEntry->entries.end(), [](const shared_simple &e1, const shared_simple &e2)
                  { return e1->getDate() < e2->getDate(); });

        TableView tableView;
        tableView.addRow({"Index", "Date", "Tag", "Amount"});

        for (size_t i = 0; i < monthEntry->entries.size(); i++)
            tableView.addRow({TableView::to_string(i), monthEntry->entries[i]->getDate(), monthEntry->entries[i]->getTag(), TableView::to_string(monthEntry->entries[i]->getAmount()) + " " + Config::appConfig.currency});

        tableView.print();
    }
};