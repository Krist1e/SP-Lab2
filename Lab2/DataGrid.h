#pragma once
#include <string>
#include <vector>
using std::vector;


class DataGrid
{
public:
    DataGrid(const vector<vector<std::wstring>>& table);
    void FillGrid();
    void DrawGrid();
private:
    vector<vector<std::wstring>> table;
};
