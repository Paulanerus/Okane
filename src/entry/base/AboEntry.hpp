#pragma once

#include "SimpleEntry.hpp"

enum class PayInterval
{
    MONTHLY,
    YEARLY,
};

class AboEntry : public SimpleEntry
{
public:
    AboEntry(std::string date, std::string tag, double amount, PayInterval interval) : SimpleEntry(date, tag, amount), m_Interval(interval) {}

    EntryType type() const noexcept override
    {
        return EntryType::ABO;
    }

    PayInterval interval() const
    {
        return m_Interval;
    }

private:
    const PayInterval m_Interval;
};

typedef std::shared_ptr<AboEntry> shared_abo;