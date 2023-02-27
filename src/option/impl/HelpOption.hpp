#pragma once

#include "../Option.hpp"
#include "rang.hpp"

#include <iostream>
#include <map>
#include <sstream>

class HelpOption : public Option
{

private:
    const std::map<std::string, std::string> options = {
        {"help", "-> Displays this information"},
        {"status", "<month> <year> -> Prints the income, expenses and balance of the provided month and year"},
        {"detailed", "<month> <year> -> Prints a table with every entry from the provided month and year"},
        {"currency", "<currency> -> Changes the currency"},
        {"add", "<amount> <tag> <date> -> Adds a specified amount and tag to a provided month and year"},
    };

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::cout << ' ' << "Okane " << rang::fgB::blue << "<option> " << rang::style::reset << "<args>\n";

        for (const auto &[option, info] : options)
            std::cout << '\t' << rang::fgB::blue << option << rang::style::reset << ' ' << info << '\n';

        std::cout << rang::fgB::yellow << "\nTip: <date>, <month> and <year> are always optional arguments and will be replaced with the current corresponding value.\n"
                  << rang::style::reset;
    }
};