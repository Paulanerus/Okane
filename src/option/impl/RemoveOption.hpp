#pragma once

#include "../../utils/regex.hpp"
#include "../../utils/time.hpp"
#include "../Option.hpp"
#include "config.hpp"
#include "entry.hpp"

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
            std::cout << rang::fgB::red << "Please provide at least an index." << rang::style::reset << std::endl;
            return;
        }

        if (!okane::rgx::matches_pnumber(args[0]))
        {
            std::cout << rang::fgB::red << "Please enter a valid index. (0, 1 or 20)" << rang::style::reset << std::endl;
            return;
        }

        std::size_t index = std::stoul(args[0]);

        std::string month = okane::time::to_string_fmt(okane::time::current_time(), "%m");
        std::string year = okane::time::to_string_fmt(okane::time::current_time(), "%Y");

        if (args.size() >= 2)
        {
            auto month_by_id = okane::time::month_from_id(okane::strings::convert_to_lowercase(args[1]));

            if (!month_by_id.has_value())
            {
                std::cout << rang::fgB::red << "Please provide a valid month. (January, Jan, 01 or 1)" << rang::style::reset << std::endl;
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
                std::cout << rang::fgB::red << "Please provide a valid year. (2022 or 2023)" << rang::style::reset << std::endl;
                return;
            }

            year = year_arg;
        }

        auto month_entry = okane::entry::find_month_by_id(okane::app_config().years, month, year);

        if (!month_entry.has_value())
        {
            std::cout << rang::fgB::yellow << "You don't have any entries for " << month << '.' << year << "." << rang::style::reset << std::endl;
            return;
        }

        if (index < month_entry->get().entries.size() && month_entry->get().entries[index].type == okane::entry::EntryType::Abo)
        {
            std::cout << rang::fgB::yellow << "The provided index belongs to an abo entry. Do you want to completely delete the abo? (Y/n)" << rang::style::reset << std::endl;

            char input = static_cast<char>(std::getchar());

            if (ALLOWED_YES.find(input) == ALLOWED_YES.end())
            {
                std::cout << rang::fgB::green << "Aborted..." << rang::style::reset << std::endl;
                return;
            }

            auto &abos = okane::app_config().abos;

            auto abo_entry = std::find_if(abos.begin(), abos.end(), [&](const okane::entry::Entry &abo)
                                          { return abo.tag == month_entry->get().entries[index].tag && abo.amount == month_entry->get().entries[index].amount; });

            if (abo_entry == abos.end())
            {
                std::cout << rang::fgB::red << "Surprise! The entry isn't there anymore... (You should not see this)" << rang::style::reset << std::endl;
                return;
            }

            abos.erase(abo_entry);

            std::cout << rang::fgB::green << "Successfully removed abo!" << rang::style::reset << std::endl;
            return;
        }

        if (!month_entry->get().erase(index))
        {
            std::cout << rang::fgB::red << "Your provided index (" << std::to_string(index) << ") is greater than the amount of entries for " << month << '.' << year << "." << rang::style::reset << std::endl;
            return;
        }

        std::cout << rang::fgB::green << "Successfully removed entry!" << rang::style::reset << std::endl;
    }

private:
    const std::unordered_set<char> ALLOWED_YES = {'y', 'Y', '\n'};
};