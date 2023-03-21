#include "Entry.hpp"

#include "../config/Config.hpp"
#include "../utils/StringUtils.hpp"

#include <sstream>

shared_simple Entry::fromString(const std::string &line)
{
    auto parts = Okane::String::splitStr(line, ';');

    return Entry::make_simple(parts[0], parts[1], std::stod(parts[2]));
}

shared_abo Entry::fromStringAbo(const std::string &line)
{
    auto parts = Okane::String::splitStr(line, ';');

    return Entry::make_abo(parts[0], parts[1], std::stod(parts[2]), parts[3] == "1" ? PayInterval::YEARLY : PayInterval::MONTHLY);
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