#pragma once

#include "rang.hpp"

#include "../Option.hpp"
#include "../../time/Time.hpp"

#include <iostream>

class StatusOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::cout << "Status" << std::endl;
    }
};