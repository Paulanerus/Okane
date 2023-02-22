#pragma once

#include <vector>
#include <string>

namespace Okane
{
    struct SimpleEntry
    {
        int64_t epoch;
        std::string tag;
        double amount;

        static SimpleEntry fromString(std::string &line);
    };

    struct MonthEntry
    {
        std::string monthNr;
        std::vector<SimpleEntry> entries;

        void operator<<(const SimpleEntry &entry);

        SimpleEntry operator[](size_t entry) const;
    };

    struct YearEntry
    {
        std::string yearNr;
        std::vector<MonthEntry> months;

        void operator<<(const MonthEntry &month);

        MonthEntry operator[](size_t month) const;
    };
}