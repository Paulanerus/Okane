#pragma once

#include "base/AboEntry.hpp"
#include "year/YearEntry.hpp"

namespace Entry
{
    shared_simple fromString(const std::string &line);

    shared_abo fromStringAbo(const std::string &line);

    shared_simple make_simple(std::string date, std::string tag, double amount);

    shared_abo make_abo(std::string date, std::string tag, double amount, PayInterval interval);

    shared_month make_month(std::string monthNr);

    shared_year make_year(std::string yearNr);

    shared_year getYear(const std::string &year);

    shared_month getMonth(const std::string &month, const std::string &year);
}
