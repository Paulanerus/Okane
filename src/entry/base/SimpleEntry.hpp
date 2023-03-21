#pragma once

#include <string>
#include <memory>

enum EntryType
{
    SIMPLE,
    ABO,
};

class SimpleEntry
{
public:
    SimpleEntry(std::string date, std::string tag, double amount) : m_Date(date), m_Tag(tag), m_Amount(amount) {}

    std::string getDate() const
    {
        return m_Date;
    }

    std::string getTag() const
    {
        return m_Tag;
    }

    double getAmount() const
    {
        return m_Amount;
    }

    virtual EntryType getType() const
    {
        return EntryType::SIMPLE;
    }

protected:
    std::string m_Date;

    std::string m_Tag;

    double m_Amount;
};

typedef std::shared_ptr<SimpleEntry> shared_simple;
