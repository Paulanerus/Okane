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

    inline tm *getCurrentTime()
    {
        return tmFromEpoch(getCurrentEpoch());
    }

    std::string toStringFMT(const tm *localTime, const std::string &fmt)
    {
        std::stringstream stream;
        stream << std::put_time(localTime, fmt.c_str());

        return stream.str();
    }

    inline std::string toString(const tm *localTime)
    {
        return toStringFMT(localTime, "%d.%m.%Y");
    }
}
