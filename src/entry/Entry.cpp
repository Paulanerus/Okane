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