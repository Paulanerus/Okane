#pragma once

#include "rang.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace okane
{
    namespace strings
    {
        template <typename T, typename U>
        inline std::string convert_to_str_with_style(T const &value, U const &style)
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << style << value << rang::style::reset;
            return stream.str();
        }

        template <typename T>
        inline std::string convert_to_str(T const &value)
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << value;
            return stream.str();
        }

        inline std::string convert_to_lowercase(std::string str)
        {
            std::transform(str.begin(), str.end(), str.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });
            return str;
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
