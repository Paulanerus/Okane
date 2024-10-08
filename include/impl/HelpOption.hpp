#pragma once

#include "option.hpp"
#include "rang.hpp"

#include <iostream>
#include <map>

class HelpOption : public okane::Option
{
public:
    const std::unordered_set<std::string> identifier() const noexcept
    {
        return std::unordered_set<std::string>{"help", "h"};
    }

    void execute([[maybe_unused]] const std::vector<std::string> &args) override
    {
        std::cout << ' ' << "Okane " << rang::fgB::blue << "<option> " << rang::style::reset << "<args>\n";

        for (const auto &[option, info] : options)
            std::cout << '\t' << rang::fgB::blue << option << rang::style::reset << ' ' << info << '\n';

        std::cout << rang::fgB::yellow << "\nTip: <date>, <month> and <year> are always optional arguments and will be replaced with the current corresponding value." << rang::style::reset << std::endl;
    }

private:
    const std::map<std::string, std::string> options = {
        {"help", "-> Displays this information"},
        {"status", "<month> <year> -> Prints the income, expenses and balance of the provided month and year"},
        {"detailed", "<month> <year> -> Prints a table with every entry from the provided month and year"},
        {"currency", "<currency> -> Changes the currency"},
        {"add", "<amount> <tag> <date> -> Adds a specified amount and tag to a provided month and year"},
        {"remove", "<index> <month> <year> -> Removes a specified entry from a provided index, month and year"},
        {"abo", "<amount> <tag> <interval> <date> -> Adds an entry that repeats at a specified interval and starts on the provided date"},
    };
};