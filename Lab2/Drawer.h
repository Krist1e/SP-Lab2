#pragma once
#include "DataGrid.h"
#include "Lab2.h"

class Drawer
{
public:
    Drawer();
    ~Drawer() = default;

    bool Initialize(HWND hwndParent);
    bool Render();
    void Resize(UINT width, UINT height);
    void Scroll(float delta);
    void SetData(const std::vector<std::vector<std::wstring>>& data);
    HRESULT CreateDeviceIndependentResources();

private:
    HRESULT CreateDeviceResources();
    HRESULT DrawTextOnWnd() const;

    HWND windowHandle;
    ID2D1FactoryPtr factory;
    ID2D1HwndRenderTargetPtr renderTarget;
    ID2D1SolidColorBrushPtr blackBrush;
    ID2D1SolidColorBrushPtr redBrush;
    
    IDWriteFactoryPtr writeFactory;
    IDWriteTextFormatPtr textFormat;
    
    DataGrid dataGrid;
};
