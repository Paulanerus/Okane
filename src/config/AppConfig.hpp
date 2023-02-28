#pragma once

#include "../entry/Entry.hpp"

#include <string>
#include <vector>

struct AppConfig
{
    std::string currency = "€";
    std::vector<std::shared_ptr<YearEntry>> years;
};