#pragma once

#include <string>
#include <regex>

namespace Okane
{
    const std::regex YEAR_REGEX{"[0-9]+"};

    const std::regex MONTH_REGEX{"(0[1-9]|1[0-2]).csv"};

    const std::regex ENTRY_REGEX{"\\d{2}.\\d{2}.\\d{4};[a-zA-Z0-9]*;[-+]?\\d*\\.?\\d+$"};

    const std::regex AMOUNT_REGEX{"[-+]?\\d*\\.?\\d+$"};

    const std::regex TAG_REGEX{"[a-zA-Z0-9]+$"};

    const std::regex DATE_REGEX{"^\\d{1,2}\\.\\d{1,2}\\.\\d+$"};

    const std::regex DAY_REGEX{"^(0?[1-9]|[1-2][0-9]|3[0-1])$"};

    inline bool matchesYear(const std::string &toMatch)
    {
        return std::regex_match(toMatch, YEAR_REGEX);
    }

    inline bool matchesMonth(const std::string &toMatch)
    {
        return std::regex_match(toMatch, MONTH_REGEX);
    }

    inline bool matchesEntry(const std::string &toMatch)
    {
        return std::regex_match(toMatch, ENTRY_REGEX);
    }

    inline bool matchesAmount(const std::string &toMatch)
    {
        return std::regex_match(toMatch, AMOUNT_REGEX);
    }

    inline bool matchesTag(const std::string &toMatch)
    {
        return std::regex_match(toMatch, TAG_REGEX);
    }

    inline bool matchesDate(const std::string &toMatch)
    {
        return std::regex_match(toMatch, DATE_REGEX);
    }

    inline bool matchesDay(const std::string &toMatch)
    {
        return std::regex_match(toMatch, DAY_REGEX);
    }
}