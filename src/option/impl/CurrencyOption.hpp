#pragma once

#include "rang.hpp"

#include "../Option.hpp"
#include "../../config/Config.hpp"

#include <iostream>

class CurrencyOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        if (args.empty())
        {
            std::cout << rang::fg::red << "Please provide a currency symbol/text like €, $ or yen" << rang::style::reset << std::endl;

            return;
        }

        Config::appConfig.currency = args.at(0);

        std::cout << rang::fg::green << "Currency set to '" << Config::appConfig.currency << "'" << rang::style::reset << std::endl;
    }
};