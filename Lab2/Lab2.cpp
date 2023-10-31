#include <chrono>
#include <windows.h>

#include "Drawer.h"
#include "FileReader.h"

Drawer drawer;
std::vector<std::vector<std::wstring>> data;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, const int nCmdShow)
{
    const FileReader reader("input.csv");
    if (!reader.Read(data))
        return 1;
    
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

    MSG messageQueue = {};
    while (GetMessage(&messageQueue, nullptr, 0, 0))
    {
        TranslateMessage(&messageQueue);
        DispatchMessage(&messageQueue);
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
                drawer.SetData(data);
                return 0;
            }
            return -1;
        }
    case WM_PAINT:
        {
            drawer.Render();
            return 0;
        }
    case WM_SIZE:
        {
            const UINT width = LOWORD(lParam);
            const UINT height = HIWORD(lParam);
            drawer.Resize(width, height);
            InvalidateRect(hWnd, nullptr, false);
            return 0;
        }
    case WM_MOUSEWHEEL:
        {
            const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
            drawer.Scroll(delta);
            InvalidateRect(hWnd, nullptr, false);
            return 0;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
