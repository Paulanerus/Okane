#pragma once

#include "../base/SimpleEntry.hpp"

#include <cstdint>
#include <string>
#include <vector>

struct MonthEntry
{
    std::string month_nr;
    std::vector<shared_simple> entries;

    MonthEntry(std::string month_nr) noexcept : month_nr(month_nr) {}

    void add(const shared_simple &entry);

    bool erase(std::size_t index);

    double income() const noexcept;

    double abos() const noexcept;

    double expenses() const noexcept;

    double balance() const noexcept;
};

typedef std::shared_ptr<MonthEntry> shared_month;