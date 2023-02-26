#pragma once

#include <string>
#include <regex>

namespace Okane
{
    const std::regex YEAR_REGEX{"[0-9]+"};

    const std::regex MONTH_REGEX{"(0[1-9]|1[0-2]).csv"};

    const std::regex ENTRY_REGEX{"\\d{2}.\\d{2}.\\d{4};[a-zA-Z0-9]*;[-+]?\\d*\\.?\\d+$"};

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
}