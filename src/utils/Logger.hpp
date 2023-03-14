#pragma once

#include "rang.hpp"
#include <string>
#include <iostream>

namespace Okane
{
    namespace Logging
    {

        template <typename T>
        inline void println(T const &obj)
        {
            std::cout << rang::fgB::green << obj << rang::style::reset << std::endl;
        }

        template <typename T>
        inline void printlnWarn(T const &obj)
        {
            std::cout << rang::fgB::yellow << obj << rang::style::reset << std::endl;
        }

        template <typename T>
        inline void printlnError(T const &obj)
        {
            std::cout << rang::fgB::red << obj << rang::style::reset << std::endl;
        }
    }

}
