#pragma once

#include "../base/SimpleEntry.hpp"

#include <string>
#include <vector>

struct MonthEntry
{
    std::string monthNr;
    std::vector<shared_simple> entries;

    MonthEntry(std::string monthNr) : monthNr(monthNr) {}

    void add(const shared_simple &entry);

    bool erase(const size_t index);

    double getIncome() const;

    double getAbos() const;

    double getExpenses() const;

    double getBalance() const;
};

typedef std::shared_ptr<MonthEntry> shared_month;