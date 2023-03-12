#pragma once

#include "RegexUtils.hpp"
#include "StringUtils.hpp"

#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <optional>
#include <algorithm>
#include <iomanip>
#include <unordered_map>

namespace Okane
{
    namespace Time
    {
        const std::string DATE_FORMAT{"%d.%m.%Y"};

        const std::unordered_map<std::string, std::vector<std::string>> MONTH_MAP = {
            {"01", {"January", "Jan", "1", "01"}},
            {"02", {"February", "Feb", "2", "02"}},
            {"03", {"March", "Mar", "3", "03"}},
            {"04", {"April", "Apr", "4", "04"}},
            {"05", {"May", "5", "05"}},
            {"06", {"June", "Jun", "6", "06"}},
            {"07", {"July", "Jul", "7", "07"}},
            {"08", {"August", "Aug", "8", "08"}},
            {"09", {"September", "Sep", "9", "09"}},
            {"10", {"October", "Oct", "10", "10"}},
            {"11", {"November", "Nov", "11", "11"}},
            {"12", {"December", "Dec", "12", "12"}},
        };

        inline bool isLeapYear(long year)
        {
            return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
        }

        inline std::optional<std::string> getMonthFromId(const std::string &id)
        {
            for (const auto &[month, ids] : MONTH_MAP)
            {
                if (std::find(ids.begin(), ids.end(), id) != ids.end())
                    return month;
            }

            return {};
        }

        inline bool getFormatDate(const std::string &date, std::string &insertTo)
        {
            if (!Okane::Regex::matchesDate(date))
                return false;

            const auto dateSplit = Okane::String::splitStr(date, '.');

            if (!Okane::Regex::matchesDay(dateSplit[0]) || !getMonthFromId(dateSplit[1]).has_value() || !Okane::Regex::matchesYear(dateSplit[2]))
                return false;

            std::stringstream formatDate;
            formatDate << std::setw(2) << std::setfill('0') << dateSplit[0] << '.'
                       << std::setw(2) << std::setfill('0') << dateSplit[1] << '.'
                       << dateSplit[2];

            insertTo = formatDate.str();

            return true;
        }

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
}
