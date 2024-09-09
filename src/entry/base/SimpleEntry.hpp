#pragma once

#include <string>
#include <memory>

enum class EntryType
{
    SIMPLE,
    ABO,
};

class SimpleEntry
{
public:
    SimpleEntry(std::string date, std::string tag, double amount) noexcept : m_Date(date), m_Tag(tag), m_Amount(amount) {}

    std::string date() const noexcept
    {
        return m_Date;
    }

    std::string tag() const noexcept
    {
        return m_Tag;
    }

    double amount() const noexcept
    {
        return m_Amount;
    }

    virtual EntryType type() const noexcept
    {
        return EntryType::SIMPLE;
    }

protected:
    const std::string m_Date;

    const std::string m_Tag;

    const double m_Amount;
};

typedef std::shared_ptr<SimpleEntry> shared_simple;
