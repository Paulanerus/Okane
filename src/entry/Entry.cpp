#include "Entry.hpp"

#include "../config/Config.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>

Okane::SimpleEntry Okane::SimpleEntry::fromString(std::string &line)
{
    std::vector<std::string> parts;

    std::stringstream stream(line);
    std::string tmp;

    while (std::getline(stream, tmp, ';'))
        parts.push_back(tmp);

    return {parts[0], parts[1], std::stod(parts[2])};
}

void Okane::MonthEntry::operator<<(const Okane::SimpleEntry &entry)
{
    entries.push_back(entry);
}

Okane::SimpleEntry Okane::MonthEntry::operator[](size_t entry) const
{
    return entries.at(entry);
}

double Okane::MonthEntry::getIncome()
{
    std::vector<Okane::SimpleEntry> positive;
    std::copy_if(entries.begin(), entries.end(), std::back_inserter(positive), [](const SimpleEntry &e)
                 { return e.amount > 0; });

    double totalIncome = 0;

    for (const auto &entry : positive)
        totalIncome += entry.amount;

    return totalIncome;
}

double Okane::MonthEntry::getExpenses()
{
    std::vector<Okane::SimpleEntry> negative;
    std::copy_if(entries.begin(), entries.end(), std::back_inserter(negative), [](const SimpleEntry &e)
                 { return e.amount < 0; });

    double totalExpenses = 0;

    for (const auto &entry : negative)
        totalExpenses += entry.amount;

    return totalExpenses;
}

double Okane::MonthEntry::getBalance()
{
    return getIncome() + getExpenses();
}

void Okane::YearEntry::operator<<(const Okane::MonthEntry &month)
{
    months.push_back(month);
}

Okane::MonthEntry Okane::YearEntry::operator[](size_t month) const
{
    return months.at(month);
}

std::optional<Okane::YearEntry> Okane::getYear(const std::string &year)
{
    auto yearIter = std::find_if(Config::appConfig.years.begin(), Config::appConfig.years.end(), [year](const Okane::YearEntry &y)
                                 { return y.yearNr == year; });

    if (yearIter == Config::appConfig.years.end())
        return {};

    return *yearIter;
}

std::optional<Okane::MonthEntry> Okane::getMonth(const std::string &month, const std::string &year)
{
    auto yearEntry = Okane::getYear(year);

    if (!yearEntry.has_value())
        return {};

    auto yearValue = yearEntry.value();

    auto monthIter = std::find_if(yearValue.months.begin(), yearValue.months.end(), [month](const Okane::MonthEntry &m)
                                  { return m.monthNr == month; });

    if (monthIter == yearValue.months.end())
        return {};

    return *monthIter;
}