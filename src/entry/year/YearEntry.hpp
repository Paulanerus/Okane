#pragma once

#include "../month/MonthEntry.hpp"

struct YearEntry
{
    std::string yearNr;
    std::vector<shared_month> months;

    YearEntry(std::string yearNr) : yearNr(yearNr) {}

    void add(const shared_month &month)
    {
        months.push_back(month);
    }
};

typedef std::shared_ptr<YearEntry> shared_year;