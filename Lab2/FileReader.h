#pragma once
#include <string>
#include <vector>

class FileReader
{
public:
    FileReader(std::string filePath, char delimiter = ',');
    ~FileReader() = default;

    bool Read(std::vector<std::vector<std::wstring>>& data) const;

private:
    std::string filePath;
    char delimiter;
    
    std::vector<std::wstring> ParseCsvLine(const std::string& line) const;
};
