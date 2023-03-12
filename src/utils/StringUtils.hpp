#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>

namespace Okane
{

    namespace String
    {
        inline std::string toLower(std::string str)
        {
            std::transform(str.begin(), str.end(), str.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });
            return str;
        }

        inline std::vector<std::string> splitStr(const std::string &str, char delimiter)
        {
            std::vector<std::string> parts;

            std::stringstream stream{str};
            std::string temp;

            while (std::getline(stream, temp, delimiter))
                parts.push_back(temp);

            return parts;
        }
    }
}
