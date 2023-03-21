#pragma once

#include "SimpleEntry.hpp"

enum PayInterval
{
    MONTHLY,
    YEARLY,
};

class AboEntry : public SimpleEntry
{
public:
    AboEntry(std::string date, std::string tag, double amount, PayInterval interval) : SimpleEntry(date, tag, amount), m_Interval(interval) {}

    EntryType getType() const override
    {
        return EntryType::ABO;
    }

    PayInterval getInterval() const
    {
        return m_Interval;
    }

private:
    PayInterval m_Interval;
};

typedef std::shared_ptr<AboEntry> shared_abo;