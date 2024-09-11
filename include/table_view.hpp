#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace okane
{
    class TableView
    {
    public:
        TableView(char row = '-', char border = '|', char corner = '+') : m_Row(row), m_Border(border), m_Corner(corner)
        {
        }

        void print();

        void add_row(const std::vector<std::string> &row);

        void operator<<(const std::string &row);

    private:
        const char m_Row, m_Border, m_Corner;

        std::size_t m_MaxRowSize{}, m_MaxRowLength{};

        std::vector<std::vector<std::string>> m_Rows;

        std::vector<std::size_t> m_LongestOfEachRow;

        inline std::size_t width(const std::string &str) const noexcept;

        inline std::size_t padding(const std::string &entry, std::size_t column_idx) const;

        inline void collect_column_longest();

        inline void set_max_row_length() noexcept;

        inline void print_row_line() const noexcept;
    };
}
