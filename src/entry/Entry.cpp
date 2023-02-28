#include "Entry.hpp"

#include "../config/Config.hpp"

#include <sstream>

shared_simple SimpleEntry::fromString(std::string &line)
{
    std::vector<std::string> parts;

    std::stringstream stream(line);
    std::string tmp;

    while (std::getline(stream, tmp, ';'))
        parts.push_back(tmp);

    return Entry::make_simple(parts[0], parts[1], std::stod(parts[2]));
}

void MonthEntry::add(const shared_simple &entry)
{
    entries.push_back(entry);
}

double MonthEntry::getIncome()
{
    double totalIncome{};

    for (const auto &entry : entries)
    {
        if (entry->amount > 0)
            totalIncome += entry->amount;
    }

    return totalIncome;
}

double MonthEntry::getExpenses()
{
    double totalExpenses{};

    for (const auto &entry : entries)
    {
        if (entry->amount < 0)
            totalExpenses += entry->amount;
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