#pragma once

#include <vector>
#include <string>

class TableView
{
public:
    TableView(char row = '-', char border = '|', char corner = '+');

    void print();

    void addRow(const std::vector<std::string> &row);

    void operator<<(const std::string &row);

private:
    size_t m_MaxRowSize{}, m_MaxRowLength{};

    char m_Row, m_Border, m_Corner;

    std::vector<std::vector<std::string>> m_Rows;

    std::vector<size_t> m_LongestOfEachRow;

    inline size_t getWidth(const std::string &str);

    inline size_t getPadding(const std::string &entry, size_t columnIndex);

    void collectColumnLongest();

    inline void setMaxRowLength();

    inline void printRowLine();
};