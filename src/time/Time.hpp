#pragma once

#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

namespace Okane
{
    const std::string DATE_FORMAT{"%d.%m.%Y"};

    inline tm *getCurrentTime()
    {
        int64_t epoch = std::time(nullptr);

        return std::localtime(&epoch);
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
