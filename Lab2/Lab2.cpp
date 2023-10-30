#include <chrono>
#include <windows.h>

#include "Drawer.h"
Drawer drawer;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, const int nCmdShow)
{
    constexpr wchar_t className[] = L"Lab2";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    const HWND hwnd = CreateWindowEx(
        0,
        className,
        L"MainWindow",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr)
        return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);


    MSG messageQueue = {};
    while (true)
    {
        if (PeekMessage(&messageQueue, nullptr, 0, 0, PM_REMOVE))
        {
            if (messageQueue.message == WM_QUIT)
                break;
            TranslateMessage(&messageQueue);
            DispatchMessage(&messageQueue);            
        }
    }
    
}

LRESULT CALLBACK WindowProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            if (drawer.Initialize(hWnd))
            {
                //drawer->SetText(L"Hello, World!");
                return 0;
            }
            return -1;
        }
    case WM_PAINT:
        {
            drawer.Render();
            break;  
        }
    case WM_SIZE:
        {
            if (wParam == SIZE_RESTORED)
            {
                const UINT width = LOWORD(lParam);
                const UINT height = HIWORD(lParam);
                drawer.Resize(width, height);
            }
            break;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}