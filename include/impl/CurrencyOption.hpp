#pragma once

#include "rang.hpp"

#include "option.hpp"
#include "config.hpp"

#include <iostream>

class CurrencyOption : public okane::Option
{

public:
    const std::unordered_set<std::string> identifier() const noexcept
    {
        return std::unordered_set<std::string>{"currency", "c"};
    }

    void execute(const std::vector<std::string> &args) override
    {
        if (args.empty())
        {
            std::cout << rang::fgB::red << "Please provide a currency symbol/text like €, $ or Yen." << rang::style::reset << std::endl;
            return;
        }

        okane::app_config().currency = args[0];

        std::cout << rang::fgB::green << "Currency set to '" << okane::app_config().currency << "'" << rang::style::reset << std::endl;
    }
};