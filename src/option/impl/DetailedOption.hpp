#pragma once

#include "../Option.hpp"

class DetailedOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::cout << "Detailed" << std::endl;
    }
};