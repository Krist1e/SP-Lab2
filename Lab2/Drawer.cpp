#include "Drawer.h"

Drawer::Drawer()
{
    windowHandle = nullptr;
}

bool Drawer::Initialize(const HWND hwnd)
{
    windowHandle = hwnd;
    if (FAILED(CreateDeviceIndependentResources()))
        return false;

    dataGrid.Initialize(writeFactory, textFormat);
    return true;
}

bool Drawer::Render()
{
    if (FAILED(CreateDeviceResources()))
        return false;

    PAINTSTRUCT ps;
    BeginPaint(windowHandle, &ps);

    renderTarget->BeginDraw();

    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    if (FAILED(DrawTextOnWnd()))
        return false;

    renderTarget->EndDraw();

    EndPaint(windowHandle, &ps);
    return true;
}

void Drawer::Resize(const UINT width, const UINT height)
{
    D2D1_SIZE_U size;
    size.width = width;
    size.height = height;

    dataGrid.SetSize(size);
    
    if (renderTarget)
        renderTarget->Resize(size);
}

void Drawer::Scroll(const float delta)
{
    dataGrid.Move(D2D1::Point2F(0, delta / 10));
}

void Drawer::SetData(const std::vector<std::vector<std::wstring>>& data)
{
    dataGrid.FillGrid(data);
}

HRESULT Drawer::CreateDeviceResources()
{
    HRESULT hr = S_OK;
    if (renderTarget == NULL)
    {
        RECT rc;
        GetClientRect(windowHandle, &rc);
        const D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(windowHandle, size),
            &renderTarget);

        if (SUCCEEDED(hr))
        {
            hr = renderTarget->CreateSolidColorBrush(
                D2D1::ColorF(
                    D2D1::ColorF::MediumVioletRed
                ),
                &blackBrush
            );
            hr = renderTarget->CreateSolidColorBrush(
                D2D1::ColorF(
                    D2D1::ColorF::OrangeRed
                ),
                &redBrush
            );
        }
    }
    return hr;
}

HRESULT Drawer::CreateDeviceIndependentResources()
{
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &factory
    );
    
    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&writeFactory)
            );
    }
    
    if (SUCCEEDED(hr))
    {
        hr = writeFactory->CreateTextFormat(
            L"TimesNewRoman",
            nullptr,                        
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            18.0f,
            L"en-us",
            &textFormat
            );
    }
    
    if (SUCCEEDED(hr))
    {
        hr = textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    }

    if (SUCCEEDED(hr))
    {
        hr = textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }
    
    return hr;
}

HRESULT Drawer::DrawTextOnWnd() const
{
    RECT rc;
    GetClientRect(windowHandle,&rc);

    dataGrid.DrawGrid(*renderTarget, *blackBrush);

    return S_OK;
}
