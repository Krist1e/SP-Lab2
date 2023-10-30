#include "Drawer.h"

Drawer::Drawer()
{
    windowHandle = nullptr;
}

bool Drawer::Initialize(const HWND hwnd)
{
    windowHandle = hwnd;
    return SUCCEEDED(CreateDeviceIndependentResources()) && SUCCEEDED(
        CreateDeviceResources());
}

void Drawer::SetText(const std::wstring& text)
{
    this->text = text;
}

std::wstring Drawer::GetText()
{
    return this->text;
}

bool Drawer::Render()
{
    if (FAILED(CreateDeviceResources()))
        return false;

    PAINTSTRUCT ps;
    BeginPaint(windowHandle, &ps);

    renderTarget->BeginDraw();

    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    DrawTextOnWnd();

    renderTarget->EndDraw();

    EndPaint(windowHandle, &ps);
    return true;
}

void Drawer::Resize(const UINT width, const UINT height) const
{
    if (textLayout)
    {
        textLayout->SetMaxWidth(static_cast<FLOAT>(width));
        textLayout->SetMaxHeight(static_cast<FLOAT>(height));
    }

    if (renderTarget)
    {
        D2D1_SIZE_U size;
        size.width = width;
        size.height = height;
        renderTarget->Resize(size);
    }
    
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
    HRESULT hr;
    
    hr = D2D1CreateFactory(
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
    
    text = L"asujdfbuhnoitkjhpogfdsaiGYUSSR";
    
    if (SUCCEEDED(hr))
    {
        hr = writeFactory->CreateTextFormat(
            L"TimesNewRoman",              
            NULL,                        
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            72.0f,
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
    
    if (SUCCEEDED(hr))
    {
        hr = writeFactory->CreateTextLayout(
            text.c_str(),  
            text.length(), 
            textFormat,  
            FLT_MAX,        
            480.0f,         
            &textLayout  
            );
    }
    
    return hr;
}

HRESULT Drawer::DrawTextOnWnd()
{
    RECT rc;

    GetClientRect(
        windowHandle,
        &rc
        );


    D2D1_POINT_2F origin = D2D1::Point2F(
        static_cast<FLOAT>(rc.top),
        static_cast<FLOAT>(rc.left)
        );



    renderTarget->DrawTextLayout(
        origin,
        textLayout,
        blackBrush
        );

    return S_OK;
}