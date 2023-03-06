#include "TableView.hpp"

#include <iostream>
#include <numeric>

TableView::TableView(char row, char border, char corner) : m_Row(row), m_Border(border), m_Corner(corner)
{
}

void TableView::print()
{
    for (auto &row : m_Rows)
        row.resize(m_MaxRowSize, " ");

    collectColumnLongest();

    setMaxRowLength();

    printRowLine();

    for (size_t i = 0; i < m_Rows.size(); i++)
    {
        std::cout << m_Border << ' ';

        for (size_t j = 0; j < m_MaxRowSize; j++)
        {
            auto entry = m_Rows[i][j];

            std::cout << entry << std::setw(getPadding(entry, j));

            if (j != m_MaxRowLength - 1)
                std::cout << ' ' << m_Border << ' ';
        }

        std::cout << '\n';
    }

    printRowLine();
}

void TableView::addRow(const std::vector<std::string> &row)
{
    if (row.size() > m_MaxRowSize)
        m_MaxRowSize = row.size();

    m_Rows.push_back(row);
}

void TableView::operator<<(const std::string &row)
{
    addRow({row});
}

inline size_t TableView::getWidth(const std::string &str)
{
    size_t width{};

    for (size_t i = 0; i < str.length(); i += std::mblen(&str[i], str.length() - i))
        width++;

    return width;
}

inline size_t TableView::getPadding(const std::string &entry, size_t columnIndex)
{
    return (m_LongestOfEachRow[columnIndex] - getWidth(entry)) + 1;
}

void TableView::collectColumnLongest()
{
    for (size_t i = 0; i < m_MaxRowSize; i++)
    {
        size_t longest{};
        for (size_t j = 0; j < m_Rows.size(); j++)
        {
            if (m_Rows[j][i].size() > longest)
                longest = m_Rows[j][i].size();
        }

        m_LongestOfEachRow.push_back(longest);
    }
}

inline void TableView::setMaxRowLength()
{
    m_MaxRowLength = 3 * m_MaxRowSize - 1;

    for (const auto &row : m_LongestOfEachRow)
        m_MaxRowLength += row;
}

inline void TableView::printRowLine()
{
    std::cout << m_Corner << std::string(m_MaxRowLength, m_Row) << m_Corner << std::endl;
}