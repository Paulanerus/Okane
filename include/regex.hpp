#pragma once

#include <regex>
#include <string>

namespace okane {
namespace rgx {
    inline const std::regex MONTH_REGEX { "(0[1-9]|1[0-2]).csv" };

    inline const std::regex ENTRY_REGEX { "\\d{2}.\\d{2}.\\d{4};[a-zA-Z0-9!@#$%^&*()_+{}|:\"\'<>?~ \\-]+;[-+]?\\d*\\.?\\d+$" };

    inline const std::regex AMOUNT_REGEX { "[-+]?\\d*\\.?\\d{0,2}$" };

    inline const std::regex TAG_REGEX { "^[a-zA-Z0-9!@#$%^&*()_+{}|:\"\'<>?~ \\-]+$" };

    inline const std::regex DATE_REGEX { "^\\d{1,2}\\.\\d{1,2}\\.\\d+$" };

    inline const std::regex DAY_REGEX { "^(0?[1-9]|[1-2][0-9]|3[0-1])$" };

    inline const std::regex PNUMBER_REGEX { "^[0-9]+$" };

    inline const std::regex ABO_REGEX { "^\\d{2}.\\d{2}.\\d{4};[a-zA-Z0-9!@#$%^&*()_+{}|:\"\'<>?~ \\-]+;[-+]?\\d*\\.?\\d+;[01]$" };

    inline const std::regex STYLE_REGEX { "\033\\[[0-9]+m" };

    inline std::string replace(const std::string& str, const std::regex& rgx, const std::string& replace)
    {
        return std::regex_replace(str, rgx, replace);
    }

    inline bool matches_month(const std::string& toMatch)
    {
        return std::regex_match(toMatch, MONTH_REGEX);
    }

    inline bool matches_entry(const std::string& toMatch)
    {
        return std::regex_match(toMatch, ENTRY_REGEX);
    }

    inline bool matches_amount(const std::string& toMatch)
    {
        return std::regex_match(toMatch, AMOUNT_REGEX);
    }

    inline bool matches_tag(const std::string& toMatch)
    {
        return std::regex_match(toMatch, TAG_REGEX);
    }

    inline bool matches_date(const std::string& toMatch)
    {
        return std::regex_match(toMatch, DATE_REGEX);
    }

    inline bool matches_day(const std::string& toMatch)
    {
        return std::regex_match(toMatch, DAY_REGEX);
    }

    inline bool matches_pnumber(const std::string& toMatch)
    {
        return std::regex_match(toMatch, PNUMBER_REGEX);
    }

    inline bool matches_abo(const std::string& toMatch)
    {
        return std::regex_match(toMatch, ABO_REGEX);
    }
}
}
