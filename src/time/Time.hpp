#pragma once

#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

namespace Okane
{
    inline int64_t getCurrentEpoch()
    {
        return std::time(nullptr);
    }

    inline tm *tmFromEpoch(const int64_t epoch)
    {
        return std::localtime(&epoch);
    }

    std::string toString(const tm *localTime)
    {
        std::stringstream stream;
        stream << std::put_time(localTime, "%d.%m.%Y");

        return stream.str();
    }
}
