#pragma once

#include <string>
#include <regex>

namespace Okane
{
    const std::regex YEAR_REGEX{"[0-9]+"};

    const std::regex MONTH_REGEX{"([1-9]|1[0-2]).csv"};

    const std::regex FILE_REGEX{"^[-+]?\\d+;[a-zA-Z0-9]*;[-+]?\\d*\\.?\\d+$"};

    /*
    bool matchesYear(const std::string &toMatch)
    {
        return std::regex_match(toMatch, YEAR_REGEX);
    }

    bool matchesMonth(const std::string &toMatch)
    {
        return std::regex_match(toMatch, MONTH_REGEX);
    }

    bool matchesFile(const std::string &toMatch)
    {
        return std::regex_match(toMatch, FILE_REGEX);
    }
    */
}