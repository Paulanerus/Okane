#pragma once

#include "base/AboEntry.hpp"
#include "year/YearEntry.hpp"

namespace Entry
{
    shared_simple from_string(const std::string &line) noexcept;

    shared_abo from_string_abo(const std::string &line) noexcept;

    shared_simple make_simple(std::string date, std::string tag, double amount) noexcept;

    shared_abo make_abo(std::string date, std::string tag, double amount, PayInterval interval) noexcept;

    shared_month make_month(std::string month_nr) noexcept;

    shared_year make_year(std::string year_nr) noexcept;

    shared_year year(const std::string &_year) noexcept;

    shared_month month(const std::string &_month, const std::string &_year) noexcept;
}
