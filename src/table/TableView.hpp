#pragma once

#include <vector>
#include <string>
#include <iostream>

class TableView
{
public:
    TableView(char row = '-', char border = '|', char corner = '+') : m_Row(row), m_Border(border), m_Corner(corner)
    {
    }

    void print()
    {
        fillRowsToSameSize();

        collectColumnLongest();

        setMaxRowLength();

        printRowLine();

        for (size_t i = 0; i < m_Rows.size(); i++)
        {
            std::cout << m_Border << ' ';

            for (size_t j = 0; j < m_MaxRowSize; j++)
            {
                auto entry = m_Rows[i][j];

                std::cout << entry << fillPadding(getPadding(entry, j));

                if (j != m_MaxRowLength - 1)
                    std::cout << ' ' << m_Border << ' ';
            }

            std::cout << '\n';
        }

        printRowLine();
    }

    void operator<<(const std::vector<std::string> &row)
    {
        if (row.size() > m_MaxRowSize)
            m_MaxRowSize = row.size();

        m_Rows.push_back(row);
    }

private:
    size_t m_MaxRowSize{}, m_MaxRowLength{};

    char m_Row, m_Border, m_Corner;

    std::vector<std::vector<std::string>> m_Rows;

    std::vector<size_t> m_LongestOfEachRow;

    inline size_t getPadding(const std::string &entry, size_t columnIndex)
    {
        return m_LongestOfEachRow[columnIndex] - entry.length();
    }

    std::string fillPadding(size_t size)
    {
        std::string padding;

        for (size_t i = 0; i < size; i++)
            padding += " ";

        return padding;
    }

    void fillRowsToSameSize()
    {
        for (auto &row : m_Rows)
        {
            while (row.size() < m_MaxRowSize)
                row.push_back(" ");
        }
    }

    void collectColumnLongest()
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

    void setMaxRowLength()
    {
        m_MaxRowLength = 3 * m_MaxRowSize - 1;

        for (const auto &size : m_LongestOfEachRow)
            m_MaxRowLength += size;
    }

    void printRowLine()
    {
        std::cout << m_Corner;
        for (size_t i = 0; i < m_MaxRowLength; i++)
            std::cout << m_Row;

        std::cout << m_Corner << '\n';
    }
};