#pragma once

#include <vector>
#include <string>
#include <memory>

class SimpleEntry
{
public:
    SimpleEntry(std::string date, std::string tag, double amount) : m_Date(date), m_Tag(tag), m_Amount(amount) {}

    static std::shared_ptr<SimpleEntry> fromString(std::string &line);

    std::string getDate() const;

    std::string getTag() const;

    double getAmount() const;

private:
    std::string m_Date;

    std::string m_Tag;

    double m_Amount;
};

typedef std::shared_ptr<SimpleEntry> shared_simple;

struct MonthEntry
{
    std::string monthNr;
    std::vector<shared_simple> entries;

    MonthEntry(std::string monthNr) : monthNr(monthNr) {}

    void add(const shared_simple &entry);

    bool erase(const size_t index);

    double getIncome();

    double getExpenses();

    double getBalance();
};

typedef std::shared_ptr<MonthEntry> shared_month;

struct YearEntry
{
    std::string yearNr;
    std::vector<shared_month> months;

    YearEntry(std::string yearNr) : yearNr(yearNr) {}

    void add(const shared_month &month);
};

typedef std::shared_ptr<YearEntry> shared_year;

namespace Entry
{
    shared_simple make_simple(std::string date, std::string tag, double amount);

    shared_month make_month(std::string monthNr);

    shared_year make_year(std::string yearNr);

    shared_year getYear(const std::string &year);

    shared_month getMonth(const std::string &month, const std::string &year);
}
