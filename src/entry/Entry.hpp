#pragma once

#include <algorithm>
#include <vector>
#include <string>

namespace Okane
{
    struct SimpleEntry
    {
        int64_t epoch;
        std::string tag;
        double amount;
    };

    struct MonthEntry
    {
        std::string monthNr;
        std::vector<SimpleEntry> entries;

        void operator<<(const SimpleEntry &entry)
        {
            entries.push_back(entry);
        }

        SimpleEntry operator[](size_t entry) const
        {
            return entries.at(entry);
        }
    };

    struct YearEntry
    {
        std::string yearNr;
        std::vector<MonthEntry> months;

        bool isMonthPresent(std::string month)
        {
            return std::any_of(months.begin(), months.end(), [month](const auto &m)
                               { return m.monthNr == month; });
        }

        void operator<<(const MonthEntry &month)
        {
            months.push_back(month);
        }

        MonthEntry operator[](size_t month) const
        {
            return months.at(month);
        }
    };
}