#pragma once

#include "../Option.hpp"
#include "../../config/Config.hpp"
#include "../../utils/Logger.hpp"

#include <iostream>

class CurrencyOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        if (args.empty())
        {
            Okane::Logging::printlnError("Please provide a currency symbol/text like €, $ or Yen.");
            return;
        }

        Config::appConfig.currency = args[0];

        Okane::Logging::println("Currency set to '" + Config::appConfig.currency + "'" );
    }
};