#pragma once

#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

namespace Okane
{
    const std::string DATE_FORMAT{"%d.%m.%Y"};

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

    inline std::string toStringFMT(const tm *localTime, const std::string &fmt)
    {
        std::stringstream stream;
        stream << std::put_time(localTime, fmt.c_str());

        return stream.str();
    }

    inline std::string toString(const tm *localTime)
    {
        return toStringFMT(localTime, DATE_FORMAT);
    }
}
