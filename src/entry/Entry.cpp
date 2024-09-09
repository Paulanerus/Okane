#include "Entry.hpp"

#include "../config/Config.hpp"
#include "../utils/strings.hpp"

#include <sstream>

shared_simple Entry::from_string(const std::string &line) noexcept
{
    auto parts = okane::strings::split_str(line, ';');

    return Entry::make_simple(parts[0], parts[1], std::stod(parts[2]));
}

shared_abo Entry::from_string_abo(const std::string &line) noexcept
{
    auto parts = okane::strings::split_str(line, ';');

    return Entry::make_abo(parts[0], parts[1], std::stod(parts[2]), parts[3] == "1" ? PayInterval::YEARLY : PayInterval::MONTHLY);
}

shared_simple Entry::make_simple(std::string date, std::string tag, double amount) noexcept
{
    return std::make_shared<SimpleEntry>(date, tag, amount);
}

shared_abo Entry::make_abo(std::string date, std::string tag, double amount, PayInterval interval) noexcept
{
    return std::make_shared<AboEntry>(date, tag, amount, interval);
}

shared_month Entry::make_month(std::string month_nr) noexcept
{
    return std::make_shared<MonthEntry>(month_nr);
}

shared_year Entry::make_year(std::string year_nr) noexcept
{
    return std::make_shared<YearEntry>(year_nr);
}

shared_year Entry::year(const std::string &_year) noexcept
{
    for (auto &yearEntry : Config::s_AppConfig.years)
    {
        if (yearEntry->year_nr == _year)
            return {yearEntry};
    }

    return nullptr;
}

shared_month Entry::month(const std::string &_month, const std::string &_year) noexcept
{
    auto yearEntry = year(_year);

    if (!yearEntry)
        return nullptr;

    for (auto &monthEntry : yearEntry->months)
    {
        if (monthEntry->month_nr == _month)
            return {monthEntry};
    }

    return nullptr;
}