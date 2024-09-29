#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace okane {
namespace entry {

    enum class EntryType {
        Normal,
        Abo,
    };

    enum class PayInterval {
        None = -1,
        Monthly = 0,
        Yearly = 1,
    };

    struct Entry {
        double amount;
        std::string tag;
        std::string date;

        EntryType type;
        PayInterval interval;
    };

    Entry make_entry(double amount, const std::string& tag, const std::string& date) noexcept;

    Entry make_abo(double amount, const std::string& tag, const std::string& date, PayInterval interval) noexcept;

    Entry make_entry_from_str(const std::string& line) noexcept;

    Entry make_abo_from_str(const std::string& line) noexcept;

    struct Month {
        std::string month_id;
        std::vector<Entry> entries;

        void add(const Entry& entry);

        bool erase(std::size_t index);

        double income() const noexcept;

        double abos() const noexcept;

        double expenses() const noexcept;

        double balance() const noexcept;
    };

    Month make_month(const std::string& month_id) noexcept;

    struct Year {
        std::string year_id;
        std::vector<Month> months;
    };

    Year make_year(const std::string& year_id) noexcept;

    std::optional<std::reference_wrapper<Year>> find_year_by_id(std::vector<Year>& years, std::string_view id) noexcept;

    std::optional<std::reference_wrapper<Month>> find_month_by_id(std::vector<Year>& years, std::string_view month_id, std::string_view year_id) noexcept;
}
}
