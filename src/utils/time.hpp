#pragma once

#include "strings.hpp"
#include "regex.hpp"

#include <unordered_map>
#include <algorithm>
#include <optional>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <string>
#include <ctime>

namespace okane
{
    namespace time
    {
        inline const std::string DATE_FORMAT{"%d.%m.%Y"};

        inline const std::unordered_map<std::string, std::vector<std::string>> MONTH_MAP = {
            {"01", {"january", "jan", "1", "01"}},
            {"02", {"february", "feb", "2", "02"}},
            {"03", {"march", "mar", "3", "03"}},
            {"04", {"april", "apr", "4", "04"}},
            {"05", {"may", "5", "05"}},
            {"06", {"june", "jun", "6", "06"}},
            {"07", {"july", "jul", "7", "07"}},
            {"08", {"august", "aug", "8", "08"}},
            {"09", {"september", "sep", "9", "09"}},
            {"10", {"october", "oct", "10", "10"}},
            {"11", {"november", "nov", "11", "11"}},
            {"12", {"december", "dec", "12", "12"}},
        };

        inline bool is_leap_year(long year)
        {
            return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
        }

        inline std::optional<std::string> month_from_id(const std::string &id)
        {
            for (auto &[month, ids] : MONTH_MAP)
            {
                if (std::find(ids.begin(), ids.end(), id) != ids.end())
                    return month;
            }

            return {};
        }

        inline bool format_date(const std::string &date, std::string &insert_to)
        {
            if (!okane::rgx::matches_date(date))
                return false;

            const auto dateSplit = okane::strings::split_str(date, '.');

            if (!okane::rgx::matches_day(dateSplit[0]) || !month_from_id(dateSplit[1]).has_value() || !okane::rgx::matches_pnumber(dateSplit[2]))
                return false;

            std::stringstream formatDate;
            formatDate << std::setw(2) << std::setfill('0') << dateSplit[0] << '.'
                       << std::setw(2) << std::setfill('0') << dateSplit[1] << '.'
                       << dateSplit[2];

            insert_to = formatDate.str();

            return true;
        }

        inline tm *current_time() noexcept
        {
            std::int64_t epoch = std::time(nullptr);

            return std::localtime(&epoch);
        }

        inline std::string to_string_fmt(const tm *local_time, const std::string &fmt) noexcept
        {
            std::stringstream stream;
            stream << std::put_time(local_time, fmt.c_str());

            return stream.str();
        }

        inline std::string to_string(const tm *local_time) noexcept
        {
            return to_string_fmt(local_time, DATE_FORMAT);
        }
    }
}
