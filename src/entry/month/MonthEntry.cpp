#include "MonthEntry.hpp"

#include "../base/AboEntry.hpp"

void MonthEntry::add(const shared_simple &entry)
{
    entries.push_back(entry);
}

bool MonthEntry::erase(const size_t index)
{
    if (index >= entries.size())
        return false;

    entries.erase(entries.begin() + index);

    return true;
}

double MonthEntry::getIncome() const
{
    double totalIncome{};

    for (const auto &entry : entries)
    {
        if (entry->getAmount() > 0)
            totalIncome += entry->getAmount();
    }

    return totalIncome;
}

double MonthEntry::getAbos() const
{
    double totalAbos{};

    for (const auto &entry : entries)
    {
        if (entry->getType() == EntryType::SIMPLE || entry->getAmount() >= 0)
            continue;

        auto abo = std::static_pointer_cast<AboEntry>(entry);

        totalAbos += abo->getAmount() / (abo->getInterval() == PayInterval::YEARLY ? 12 : 1);
    }

    return totalAbos;
}

double MonthEntry::getExpenses() const
{
    double totalExpenses{};

    for (const auto &entry : entries)
    {
        if (entry->getAmount() < 0 && entry->getType() == EntryType::SIMPLE)
            totalExpenses += entry->getAmount();
    }

    return totalExpenses;
}

double MonthEntry::getBalance() const
{
    return getIncome() + getExpenses() + getAbos();
}