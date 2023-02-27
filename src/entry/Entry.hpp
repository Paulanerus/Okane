#pragma once

#include <vector>
#include <string>
#include <optional>

namespace Okane
{
    struct SimpleEntry
    {
        std::string date;
        std::string tag;
        double amount;

        static SimpleEntry fromString(std::string &line);
    };

    struct MonthEntry
    {
        std::string monthNr;
        std::vector<SimpleEntry> entries;

        void operator<<(const SimpleEntry &entry);

        SimpleEntry operator[](size_t entry) const;

        double getIncome();

        double getExpenses();

        double getBalance();
    };

    struct YearEntry
    {
        std::string yearNr;
        std::vector<MonthEntry> months;

        void operator<<(const MonthEntry &month);

        MonthEntry operator[](size_t month) const;
    };

    std::optional<YearEntry> getYear(const std::string &year);
    
    std::optional<MonthEntry> getMonth(const std::string &month, const std::string &year);
}