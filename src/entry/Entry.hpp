#pragma once

#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

namespace Okane
{
    struct SimpleEntry
    {
        int64_t epoch;
        std::string tag;
        double amount;

        static SimpleEntry fromString(std::string &line)
        {
            std::vector<std::string> parts;

            std::stringstream stream(line);
            std::string tmp;

            while (std::getline(stream, tmp, ';'))
                parts.push_back(tmp);

            return {std::stol(parts[0]), parts[1], std::stod(parts[2])};
        }
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