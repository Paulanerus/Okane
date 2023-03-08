#include "Entry.hpp"

#include "../config/Config.hpp"
#include "../utils/StringUtils.hpp"

#include <sstream>

std::string SimpleEntry::getDate() const 
{
    return m_Date;
}

std::string SimpleEntry::getTag() const
{
    return m_Tag;
}

double SimpleEntry::getAmount() const
{
    return m_Amount;
}

EntryType SimpleEntry::getType() const
{
    return EntryType::SIMPLE;
}

EntryType AboEntry::getType() const
{
    return EntryType::ABO;
}

PayInterval AboEntry::getInterval() const
{
    return m_Interval;
}

shared_simple Entry::fromString(std::string &line)
{
    auto parts = Okane::String::split_str(line, ';');

    return Entry::make_simple(parts[0], parts[1], std::stod(parts[2]));
}

void MonthEntry::add(const shared_simple &entry)
{
    entries.push_back(entry);
}

bool MonthEntry::erase(const size_t index)
{
    if (index >= entries.size())
    {
        return false;
    }

    entries.erase(entries.begin() + index);

    return true;
}

double MonthEntry::getIncome()
{
    double totalIncome{};

    for (const auto &entry : entries)
    {
        if (entry->getAmount() > 0)
            totalIncome += entry->getAmount();
    }

    return totalIncome;
}

double MonthEntry::getExpenses()
{
    double totalExpenses{};

    for (const auto &entry : entries)
    {
        if (entry->getAmount() < 0)
            totalExpenses += entry->getAmount();
    }

    return totalExpenses;
}

double MonthEntry::getBalance()
{
    return getIncome() + getExpenses();
}

void YearEntry::add(const shared_month &month)
{
    months.push_back(month);
}

shared_simple Entry::make_simple(std::string date, std::string tag, double amount)
{
    return std::make_shared<SimpleEntry>(date, tag, amount);
}

shared_abo Entry::make_abo(std::string date, std::string tag, double amount, PayInterval interval)
{
    return std::make_shared<AboEntry>(date, tag, amount, interval);
}

shared_month Entry::make_month(std::string monthNr)
{
    return std::make_shared<MonthEntry>(monthNr);
}

shared_year Entry::make_year(std::string yearNr)
{
    return std::make_shared<YearEntry>(yearNr);
}

shared_year Entry::getYear(const std::string &year)
{
    for (const auto &yearEntry : Config::appConfig.years)
    {
        if (yearEntry->yearNr == year)
            return {yearEntry};
    }

    return nullptr;
}

shared_month Entry::getMonth(const std::string &month, const std::string &year)
{
    auto yearEntry = getYear(year);

    if (!yearEntry)
        return nullptr;

    for (const auto &monthEntry : yearEntry->months)
    {
        if (monthEntry->monthNr == month)
            return {monthEntry};
    }

    return nullptr;
}