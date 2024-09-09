#pragma once

#include "../month/MonthEntry.hpp"

struct YearEntry
{
    std::string year_nr;
    std::vector<shared_month> months;

    YearEntry(std::string year_nr) : year_nr(year_nr) {}

    void add(const shared_month &month)
    {
        months.push_back(month);
    }
};

typedef std::shared_ptr<YearEntry> shared_year;