#pragma once
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <comdef.h>
#include <string>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

_COM_SMARTPTR_TYPEDEF(ID2D1Factory, __uuidof(ID2D1Factory));
_COM_SMARTPTR_TYPEDEF(ID2D1HwndRenderTarget, __uuidof(ID2D1HwndRenderTarget));
_COM_SMARTPTR_TYPEDEF(ID2D1SolidColorBrush, __uuidof(ID2D1SolidColorBrush));
_COM_SMARTPTR_TYPEDEF(IDWriteFactory, __uuidof(IDWriteFactory));
_COM_SMARTPTR_TYPEDEF(IDWriteTextFormat, __uuidof(IDWriteTextFormat));
_COM_SMARTPTR_TYPEDEF(IDWriteTextLayout, __uuidof(IDWriteTextLayout));

class Drawer
{
public:
    Drawer();
    ~Drawer() = default;

    bool Initialize(HWND hwndParent);
    bool Render();
    void Resize(UINT width, UINT height) const;
    void SetText(const std::wstring& text);
    std::wstring GetText();
    HRESULT DrawD2DContent();
    HRESULT CreateDeviceIndependentResources();

private:
    HRESULT CreateDeviceResources();
    HRESULT DrawTextOnWnd();

    HWND windowHandle;
    ID2D1FactoryPtr factory;
    ID2D1HwndRenderTargetPtr renderTarget;
    ID2D1SolidColorBrushPtr blackBrush;
    ID2D1SolidColorBrushPtr redBrush;
    
    IDWriteFactoryPtr writeFactory;
    IDWriteTextFormatPtr textFormat;
    IDWriteTextLayoutPtr textLayout;
    
    std::wstring text;
};
