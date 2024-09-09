#pragma once

#include "../../config/Config.hpp"
#include "../../entry/Entry.hpp"
#include "../../utils/regex.hpp"
#include "../../utils/time.hpp"
#include "../Option.hpp"

#include <unordered_set>
#include <iostream>
#include <cstdint>

class RemoveOption : public Option
{
public:
    void execute(const std::vector<std::string> &args) override
    {
        if (args.size() < 1)
        {
            Okane::Logging::printlnError("Please provide at least an index.");
            return;
        }

        if (!okane::rgx::matches_pnumber(args[0]))
        {
            Okane::Logging::printlnError("Please enter a valid index. (0, 1 or 20)");
            return;
        }

        std::size_t index = std::stoul(args[0]);

        std::string month = okane::time::to_string_fmt(okane::time::current_time(), "%m");
        std::string year = okane::time::to_string_fmt(okane::time::current_time(), "%Y");

        if (args.size() >= 2)
        {
            auto month_by_id = okane::time::month_from_id(okane::strings::to_lower(args[1]));

            if (!month_by_id.has_value())
            {
                Okane::Logging::printlnError("Please provide a valid month. (January, Jan, 01 or 1)");
                return;
            }

            month = month_by_id.value();

            if (args.size() == 2)
            {
                return;
            }

            auto year_arg = args[2];

            if (!okane::rgx::matches_pnumber(year_arg))
            {
                Okane::Logging::printlnError("Please provide a valid year. (2022 or 2023)");
                return;
            }

            year = year_arg;
        }

        auto month_entry = Entry::month(month, year);

        if (!month_entry)
        {
            Okane::Logging::printlnWarn("You don't have any entries for " + month + '.' + year + ".");
            return;
        }

        if (index < month_entry->entries.size() && month_entry->entries[index]->type() == EntryType::ABO)
        {
            Okane::Logging::printlnWarn("The provided index belongs to an abo entry. Do you want to completely delete the abo? (Y/n)");

            char input = std::getchar();

            if (ALLOWED_YES.find(input) == ALLOWED_YES.end())
            {
                Okane::Logging::println("Aborted...");
                return;
            }

            auto cast_ptr = std::static_pointer_cast<AboEntry>(month_entry->entries[index]);

            auto abo_entry = std::find_if(Config::s_AppConfig.abos.begin(), Config::s_AppConfig.abos.end(), [cast_ptr](const shared_abo &abo)
                                          { return abo->tag() == cast_ptr->tag() && abo->amount() == cast_ptr->amount(); });

            if (abo_entry == Config::s_AppConfig.abos.end())
            {
                Okane::Logging::printlnError("Surprise! The entry isn't there anymore... (You should not see this)");
                return;
            }

            Config::s_AppConfig.abos.erase(abo_entry);

            Okane::Logging::println("Successfully removed abo!");
            return;
        }

        if (!month_entry->erase(index))
        {
            Okane::Logging::printlnError("Your provided index (" + std::to_string(index) + ") is greater than the amount of entries for " + month + '.' + year + ".");
            return;
        }

        Okane::Logging::println("Successfully removed entry!");
    }

private:
    const std::unordered_set<char> ALLOWED_YES = {'y', 'Y', '\n'};
};