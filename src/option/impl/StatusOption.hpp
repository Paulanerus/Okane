#pragma once

#include "../Option.hpp"

class StatusOption : public Option
{

public:
    void execute(const std::vector<std::string> &args) override
    {
        std::cout << "Status" << std::endl;
    }
};