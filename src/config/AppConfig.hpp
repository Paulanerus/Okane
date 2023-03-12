#pragma once

#include "../entry/Entry.hpp"

#include <string>
#include <vector>

struct AppConfig
{
    std::string currency = "€";
    std::vector<shared_year> years;
    std::vector<shared_abo> abos;
};