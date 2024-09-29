#include "table_view.hpp"

#include "regex.hpp"

#include <iomanip>
#include <iostream>
#include <numeric>

namespace okane {
void TableView::print()
{
    for (auto& row : m_Rows)
        row.resize(m_MaxRowSize, " ");

    collect_column_longest();

    set_max_row_length();

    print_row_line();

    for (std::size_t i {}; i < m_Rows.size(); i++) {
        std::cout << m_Border << ' ';

        for (std::size_t j {}; j < m_MaxRowSize; j++) {
            auto entry = m_Rows[i][j];

            std::cout << entry << std::setw(padding(entry, j));

            if (j != m_MaxRowLength - 1)
                std::cout << ' ' << m_Border << ' ';
        }

        std::cout << '\n';
    }

    print_row_line();
}

void TableView::add_row(const std::vector<std::string>& row)
{
    if (row.size() > m_MaxRowSize)
        m_MaxRowSize = row.size();

    m_Rows.push_back(row);
}

void TableView::operator<<(const std::string& row)
{
    add_row({ row });
}

inline std::size_t TableView::width(const std::string& str) const noexcept
{
    std::size_t width {};

    for (std::size_t i {}; i < str.length(); i += std::mblen(&str[i], str.length() - i))
        width++;

    return width;
}

inline std::size_t TableView::padding(const std::string& entry, std::size_t column_idx) const
{
    return (m_LongestOfEachRow[column_idx] - width(okane::rgx::replace(entry, okane::rgx::STYLE_REGEX, ""))) + 1;
}

inline void TableView::collect_column_longest()
{
    for (std::size_t i {}; i < m_MaxRowSize; i++) {
        std::size_t longest {};
        for (std::size_t j {}; j < m_Rows.size(); j++) {
            auto no_color_rgx = okane::rgx::replace(m_Rows[j][i], okane::rgx::STYLE_REGEX, "");

            if (no_color_rgx.size() > longest)
                longest = no_color_rgx.size();
        }

        m_LongestOfEachRow.push_back(longest);
    }
}

inline void TableView::set_max_row_length() noexcept
{
    m_MaxRowLength = 3 * m_MaxRowSize - 1;

    for (const auto& row : m_LongestOfEachRow)
        m_MaxRowLength += row;
}

inline void TableView::print_row_line() const noexcept
{
    std::cout << m_Corner << std::string(m_MaxRowLength, m_Row) << m_Corner << std::endl;
}
}
