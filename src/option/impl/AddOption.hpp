#pragma once

#include "../Option.hpp"

class AddOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::cout << "Add" << std::endl;
    }
};