#include "entry.hpp"

#include "strings.hpp"

namespace okane
{
    namespace entry
    {
        Entry make_entry(double amount, const std::string &tag, const std::string &date) noexcept
        {
            return Entry{amount, tag, date, EntryType::Normal, PayInterval::None};
        }

        Entry make_abo(double amount, const std::string &tag, const std::string &date, PayInterval interval) noexcept
        {
            return Entry{amount, tag, date, EntryType::Abo, interval};
        }

        Entry make_entry_from_str(const std::string &line) noexcept
        {
            auto parts = okane::strings::split_str(line, ';');

            return make_entry(std::stod(parts[2]), parts[1], parts[0]);
        }

        Entry make_abo_from_str(const std::string &line) noexcept
        {
            auto parts = okane::strings::split_str(line, ';');

            return make_abo(std::stod(parts[2]), parts[1], parts[0], parts[3] == "1" ? PayInterval::Yearly : PayInterval::Monthly);
        }

        Month make_month(const std::string &month_id) noexcept
        {
            return Month{month_id, std::vector<Entry>{}};
        }

        Year make_year(const std::string &year_id) noexcept
        {
            return Year{year_id, std::vector<Month>{}};
        }

        std::optional<std::reference_wrapper<Year>> find_year_by_id(std::vector<Year> &years, std::string_view id) noexcept
        {
            for (auto &year : years)
            {
                if (year.year_id == id)
                    return year;
            }

            return std::nullopt;
        }

        std::optional<std::reference_wrapper<Month>> find_month_by_id(std::vector<Year> &years, std::string_view month_id, std::string_view year_id) noexcept
        {
            auto year_opt = find_year_by_id(years, year_id);

            if (!year_opt.has_value())
                return std::nullopt;

            for (auto &month_entry : year_opt->get().months)
            {
                if (month_entry.month_id == month_id)
                    return month_entry;
            }

            return std::nullopt;
        }

        void Month::add(const Entry &entry)
        {
            entries.push_back(entry);
        }

        bool Month::erase(std::size_t index)
        {
            if (index >= entries.size())
                return false;

            entries.erase(entries.begin() + index);

            return true;
        }

        double Month::income() const noexcept
        {
            double total_income{};
            for (auto &entry : entries)
            {
                if (entry.amount > 0)
                    total_income += entry.amount;
            }

            return total_income;
        }

        double Month::abos() const noexcept
        {
            double total_abos{};
            for (auto &entry : entries)
            {
                if (entry.type == EntryType::Normal || entry.amount >= 0)
                    continue;

                total_abos += entry.amount / (entry.interval == PayInterval::Yearly ? 12 : 1);
            }

            return total_abos;
        }

        double Month::expenses() const noexcept
        {
            double total_expenses{};
            for (auto &entry : entries)
            {
                if (entry.amount < 0 && entry.type == EntryType::Normal)
                    total_expenses += entry.amount;
            }

            return total_expenses;
        }

        double Month::balance() const noexcept
        {
            return income() + expenses() + abos();
        }
    }
}