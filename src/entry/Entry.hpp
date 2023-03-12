#pragma once

#include <string>
#include <vector>
#include <memory>

enum EntryType
{
    SIMPLE,
    ABO,
};

enum PayInterval
{
    MONTHLY,
    YEARLY,
};

class SimpleEntry
{
public:
    SimpleEntry(std::string date, std::string tag, double amount) : m_Date(date), m_Tag(tag), m_Amount(amount) {}

    std::string getDate() const;

    std::string getTag() const;

    double getAmount() const;

    virtual EntryType getType() const;

protected:
    std::string m_Date;

    std::string m_Tag;

    double m_Amount;
};

typedef std::shared_ptr<SimpleEntry> shared_simple;

class AboEntry : public SimpleEntry
{
public:
    AboEntry(std::string date, std::string tag, double amount, PayInterval interval) : SimpleEntry(date, tag, amount), m_Interval(interval) {}

    EntryType getType() const override;

    PayInterval getInterval() const;

    void setAmount(double amount);

private:
    PayInterval m_Interval;
};

typedef std::shared_ptr<AboEntry> shared_abo;

struct MonthEntry
{
    std::string monthNr;
    std::vector<shared_simple> entries;

    MonthEntry(std::string monthNr) : monthNr(monthNr) {}

    void add(const shared_simple &entry);

    bool erase(const size_t index);

    double getIncome() const;

    double getAbos() const;

    double getExpenses() const;

    double getBalance() const;
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
    shared_simple fromString(const std::string &line);

    shared_abo fromStringAbo(const std::string &line);

    shared_simple make_simple(std::string date, std::string tag, double amount);

    shared_abo make_abo(std::string date, std::string tag, double amount, PayInterval interval);

    shared_month make_month(std::string monthNr);

    shared_year make_year(std::string yearNr);

    shared_year getYear(const std::string &year);

    shared_month getMonth(const std::string &month, const std::string &year);
}
