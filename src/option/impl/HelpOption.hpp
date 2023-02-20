#pragma once

#include "../Option.hpp"

class HelpOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::cout << "Help" << std::endl;
    }
};