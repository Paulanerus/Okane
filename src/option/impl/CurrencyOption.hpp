#pragma once

#include "../../config/Config.hpp"
#include "../Option.hpp"

#include <iostream>

class CurrencyOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        if (args.empty())
        {
            std::cout << rang::fgB::red << "Please provide a currency symbol/text like €, $ or Yen." << rang::style::reset << std::endl;
            return;
        }

        Config::s_AppConfig.currency = args[0];

        std::cout << rang::fgB::green << "Currency set to '" << Config::s_AppConfig.currency << "'" << rang::style::reset << std::endl;
    }
};