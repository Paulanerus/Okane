#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <sstream>

class TableView
{
public:
    TableView(char row = '-', char border = '|', char corner = '+') : m_Row(row), m_Border(border), m_Corner(corner)
    {
    }

    template <typename T>
    static std::string to_string(T const &value)
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }

    void print()
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

    void operator<<(const std::vector<std::string> &row)
    {
        if (row.size() > m_MaxRowSize)
            m_MaxRowSize = row.size();

        m_Rows.push_back(row);
    }

    void operator<<(const std::string &row)
    {
        m_MaxRowSize = 1;

        m_Rows.push_back(std::vector<std::string>{row});
    }

private:
    size_t m_MaxRowSize{}, m_MaxRowLength{};

    char m_Row, m_Border, m_Corner;

    std::vector<std::vector<std::string>> m_Rows;

    std::vector<size_t> m_LongestOfEachRow;

    inline size_t getPadding(const std::string &entry, size_t columnIndex)
    {
        return (m_LongestOfEachRow[columnIndex] - entry.length()) + 1;
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

    inline void setMaxRowLength()
    {
        m_MaxRowLength = 3 * m_MaxRowSize - 1 + std::accumulate(m_LongestOfEachRow.begin(), m_LongestOfEachRow.end(), 0);
    }

    inline void printRowLine()
    {
        std::cout << m_Corner << std::string(m_MaxRowLength, m_Row) << m_Corner << std::endl;
    }
};