#include "DataGrid.h"

DataGrid::DataGrid(): size(), position()
{
}

bool DataGrid::Initialize(const IDWriteFactoryPtr& writeFactory, const IDWriteTextFormatPtr& textFormat)
{
    this->writeFactory = writeFactory;
    this->textFormat = textFormat;
    return true;
}

void DataGrid::FillGrid(const vector<vector<std::wstring>>& data)
{
    for (const auto& row : data)
    {
        vector<IDWriteTextLayoutPtr> rowCells;
        for (const auto& cell : row)
        {
            IDWriteTextLayoutPtr textLayout;
            writeFactory->CreateTextLayout(cell.c_str(), cell.size(), textFormat,
                                           FLT_MAX, FLT_MAX, &textLayout);
            rowCells.push_back(textLayout);
        }
        cells.push_back(rowCells);
    }
}

D2D1_SIZE_F DataGrid::SetCellWidth(const float colWidth, const int row, const int col) const
{
    auto cellSize = D2D1::SizeF(colWidth, 0);
    cells[row][col]->SetMaxWidth(cellSize.width);
    DWRITE_TEXT_METRICS textMetrics;
    cells[row][col]->GetMetrics(&textMetrics);
    cellSize.height = textMetrics.height;
    cells[row][col]->SetMaxHeight(cellSize.height);
    return cellSize;
}

void DataGrid::DrawGrid(ID2D1RenderTarget& renderTarget, ID2D1SolidColorBrush& brush) const
{
    const int colCount = static_cast<int>(cells[0].size());
    const int rowCount = static_cast<int>(cells.size());
    
    const float colWidth = size.width / static_cast<float>(colCount);
    
    float tableHeight = 0.0f;
    float rowStartY = position.y;
    for (int i = 0; i < rowCount; i++)
    {
        float rowHeight = 0.0f;
        float colStartX = 0.0f;

        for (int j = 0; j < colCount; j++)
        {
            const D2D1_POINT_2F cellPosition = D2D1::Point2F(position.x + colStartX, rowStartY);
            const D2D1_SIZE_F cellSize = SetCellWidth(colWidth, i, j);

            renderTarget.DrawTextLayout(cellPosition, cells[i][j], &brush);
            
            if (cellSize.height > rowHeight)
                rowHeight = cellSize.height;
            colStartX += colWidth;
        }

        rowStartY += rowHeight;
        tableHeight = rowStartY;
        renderTarget.DrawLine(D2D1::Point2F(position.x, rowStartY), D2D1::Point2F(position.x + size.width, rowStartY),
                              &brush);
    }

    float colStartX = colWidth;
    int colIndex = 1;

    while (colIndex++ < colCount)
    {
        renderTarget.DrawLine(D2D1::Point2F(position.x + colStartX, position.y),
                              D2D1::Point2F(position.x + colStartX, tableHeight), &brush);
        colStartX += colWidth;
    }
}

void DataGrid::SetSize(D2D1_SIZE_U size)
{
    this->size = D2D1::SizeF(static_cast<float>(size.width), static_cast<float>(size.height));
}

void DataGrid::Move(const D2D1_POINT_2F offset)
{
    position.x += offset.x;
    position.y += offset.y;
    if (position.y > 0)
        position.y = 0;
}
