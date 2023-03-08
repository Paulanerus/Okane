#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

namespace Okane
{

    namespace String
    {

        template <typename T>
        inline std::string toString(T const &value)
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << value;
            return stream.str();
        }

        inline std::vector<std::string> split_str(const std::string &str, char delimiter)
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
