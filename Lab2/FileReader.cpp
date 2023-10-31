#include "FileReader.h"

#include <fstream>
#include <sstream>

FileReader::FileReader(std::string filePath, const char delimiter) : filePath(std::move(filePath)), delimiter(delimiter)
{
}

std::vector<std::wstring> FileReader::ParseCsvLine(const std::string& line) const
{
    std::vector<std::wstring> row;
    std::stringstream lineStream(line);
    std::string cell;
    bool isInsideQuotedCell = false;
    std::string currentCell;

    while (std::getline(lineStream, cell, delimiter))
    {
        if (!isInsideQuotedCell)
        {
            if (cell.empty())
            {
                row.emplace_back(L"");
            }
            else if (cell.front() == '"')
            {
                if (cell.back() == '"')
                {
                    row.emplace_back(cell.begin() + 1, cell.end() - 1);
                }
                else
                {
                    isInsideQuotedCell = true;
                    currentCell = cell.substr(1);
                }
            }
            else
            {
                row.emplace_back(cell.begin(), cell.end());
            }
        }
        else
        {
            if (cell.back() == '"')
            {
                isInsideQuotedCell = false;
                currentCell += delimiter + cell.substr(0, cell.size() - 1);
                row.emplace_back(currentCell.begin(), currentCell.end());
            }
            else
            {
                currentCell += delimiter + cell;
            }
        }
    }

    return row;
}

bool FileReader::Read(std::vector<std::vector<std::wstring>>& data) const
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return false;

    std::string line;
    while (std::getline(file, line))
        data.emplace_back(ParseCsvLine(line));

    file.close();
    return true;
}
