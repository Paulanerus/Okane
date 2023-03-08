#pragma once

#include "../Option.hpp"
#include "../../utils/OkaneUtils.hpp"
#include "../../entry/Entry.hpp"
#include "../../table/TableView.hpp"
#include "../../config/Config.hpp"

#include <iostream>

class DetailedOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::string month = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%m");
        std::string year = Okane::Time::toStringFMT(Okane::Time::getCurrentTime(), "%Y");

        if (args.size() == 1)
        {
            auto monthById = Okane::Time::getMonthFromId(args.at(0));

            if (!monthById.has_value())
            {
                std::cout << "Please provide a valid Month (January, Jan, 01 or 1)" << std::endl;
                return;
            }

            month = monthById.value();
        }
        else if (args.size() > 1)
        {
            auto monthById = Okane::Time::getMonthFromId(args.at(0));

            if (!monthById.has_value())
            {
                std::cout << "Please provide a valid Month (January, Jan, 01 or 1)" << std::endl;
                return;
            }

            month = monthById.value();

            auto yearArg = args.at(1);

            if (!Okane::Regex::matchesYear(yearArg))
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
            tableView.addRow({Okane::String::toString(i), monthEntry->entries[i]->getDate(), monthEntry->entries[i]->getTag(), Okane::String::toString(monthEntry->entries[i]->getAmount()) + " " + Config::appConfig.currency});

        tableView.print();
    }
};