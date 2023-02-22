#include "Entry.hpp"

#include "../config/Config.hpp"

#include <iostream>
#include <sstream>

Okane::SimpleEntry Okane::SimpleEntry::fromString(std::string &line)
{
    std::vector<std::string> parts;

    std::stringstream stream(line);
    std::string tmp;

    while (std::getline(stream, tmp, ';'))
        parts.push_back(tmp);

    return {std::stol(parts[0]), parts[1], std::stod(parts[2])};
}

void Okane::MonthEntry::operator<<(const Okane::SimpleEntry &entry)
{
    entries.push_back(entry);
}

Okane::SimpleEntry Okane::MonthEntry::operator[](size_t entry) const
{
    return entries.at(entry);
}

void Okane::YearEntry::operator<<(const Okane::MonthEntry &month)
{
    months.push_back(month);
}

Okane::MonthEntry Okane::YearEntry::operator[](size_t month) const
{
    return months.at(month);
}

std::optional<Okane::MonthEntry> Okane::getMonth(const std::string &month, const std::string &year)
{
    auto yearIter = std::find_if(Config::appConfig.years.begin(), Config::appConfig.years.end(), [year](const Okane::YearEntry &y)
                                 { return y.yearNr == year; });

    if (yearIter == Config::appConfig.years.end())
        return {};

    auto yearEntry = *yearIter;

    auto monthIter = std::find_if(yearEntry.months.begin(), yearEntry.months.end(), [month](const Okane::MonthEntry &m)
                                  { return m.monthNr == month; });

    if (monthIter == yearEntry.months.end())
        return {};

    return *monthIter;
}