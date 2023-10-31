#pragma once
#include "Lab2.h"
#include <string>
#include <vector>
using std::vector;

class DataGrid
{
public:
    DataGrid();
    bool Initialize(const IDWriteFactoryPtr& writeFactory, const IDWriteTextFormatPtr& textFormat);
    void FillGrid(const vector<vector<std::wstring>>& data);
    void DrawGrid(ID2D1RenderTarget& renderTarget, ID2D1SolidColorBrush& brush) const;
    void SetSize(D2D1_SIZE_U size);
    void Move(D2D1_POINT_2F offset);

private:
    IDWriteFactoryPtr writeFactory;
    IDWriteTextFormatPtr textFormat;

    D2D1_SIZE_F size;
    D2D1_POINT_2F position;
    vector<vector<IDWriteTextLayoutPtr>> cells;
    
    D2D1_SIZE_F SetCellWidth(float colWidth, int row, int col) const;
};
