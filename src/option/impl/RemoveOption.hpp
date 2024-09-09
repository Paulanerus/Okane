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
            auto month_by_id = okane::time::month_from_id(okane::strings::to_lower(args[1]));

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

        auto month_entry = Entry::month(month, year);

        if (!month_entry)
        {
            std::cout << rang::fgB::yellow << "You don't have any entries for " << month << '.' << year << "." << rang::style::reset << std::endl;
            return;
        }

        if (index < month_entry->entries.size() && month_entry->entries[index]->type() == EntryType::ABO)
        {
            std::cout << rang::fgB::yellow << "The provided index belongs to an abo entry. Do you want to completely delete the abo? (Y/n)" << rang::style::reset << std::endl;

            char input = std::getchar();

            if (ALLOWED_YES.find(input) == ALLOWED_YES.end())
            {
                std::cout << rang::fgB::green << "Aborted..." << rang::style::reset << std::endl;
                return;
            }

            auto cast_ptr = std::static_pointer_cast<AboEntry>(month_entry->entries[index]);

            auto abo_entry = std::find_if(Config::s_AppConfig.abos.begin(), Config::s_AppConfig.abos.end(), [cast_ptr](const shared_abo &abo)
                                          { return abo->tag() == cast_ptr->tag() && abo->amount() == cast_ptr->amount(); });

            if (abo_entry == Config::s_AppConfig.abos.end())
            {
                std::cout << rang::fgB::red << "Surprise! The entry isn't there anymore... (You should not see this)" << rang::style::reset << std::endl;
                return;
            }

            Config::s_AppConfig.abos.erase(abo_entry);

            std::cout << rang::fgB::green << "Successfully removed abo!" << rang::style::reset << std::endl;
            return;
        }

        if (!month_entry->erase(index))
        {
            std::cout << rang::fgB::red << "Your provided index (" << std::to_string(index) << ") is greater than the amount of entries for " << month << '.' << year << "." << rang::style::reset << std::endl;
            return;
        }

        std::cout << rang::fgB::green << "Successfully removed entry!" << rang::style::reset << std::endl;
    }

private:
    const std::unordered_set<char> ALLOWED_YES = {'y', 'Y', '\n'};
};