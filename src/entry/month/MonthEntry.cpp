#include "MonthEntry.hpp"

#include "../base/AboEntry.hpp"

void MonthEntry::add(const shared_simple &entry)
{
    entries.push_back(entry);
}

bool MonthEntry::erase(std::size_t index)
{
    if (index >= entries.size())
        return false;

    entries.erase(entries.begin() + index);

    return true;
}

double MonthEntry::income() const noexcept
{
    double total_income{};
    for (auto &entry : entries)
    {
        if (entry->amount() > 0)
            total_income += entry->amount();
    }

    return total_income;
}

double MonthEntry::abos() const noexcept
{
    double total_abos{};
    for (auto &entry : entries)
    {
        if (entry->type() == EntryType::SIMPLE || entry->amount() >= 0)
            continue;

        auto abo = std::static_pointer_cast<AboEntry>(entry);

        total_abos += abo->amount() / (abo->interval() == PayInterval::YEARLY ? 12 : 1);
    }

    return total_abos;
}

double MonthEntry::expenses() const noexcept
{
    double total_expenses{};
    for (auto &entry : entries)
    {
        if (entry->amount() < 0 && entry->type() == EntryType::SIMPLE)
            total_expenses += entry->amount();
    }

    return total_expenses;
}

double MonthEntry::balance() const noexcept
{
    return income() + expenses() + abos();
}