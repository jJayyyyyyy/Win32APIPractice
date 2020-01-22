#include <windows.h>
#include <math.h>

// gcc template.c -lgdi32 -mwindows -o template

#define RED RGB(255,0,0)
#define pi 3.1415926

struct MyPoint {
    int x;
    int y;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("MyWin32App");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if ( !RegisterClass(&wndclass) )
    {
        MessageBox(NULL, TEXT("This program only runs on Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }
    
    hwnd = CreateWindow(szAppName,
        TEXT("Dot"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while ( GetMessage(&msg, NULL, 0, 0) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void swap(int * x, int * y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void DrawDot(const HDC hdc, int x, int y)
{
    SetPixel(hdc, x, y, RED);
}

void DrawLine(const HDC hdc)
{
    for ( int i = 0; i < 50; i++ )
    {
        SetPixel(hdc, 100 + i, 50, RED);
    }
    for ( int i = 0; i < 100; i++ )
    {
        SetPixel(hdc, 150, 50 + i, RED);
    }
}

void DrawCircle(const HDC hdc, const int centerX, const int centerY, const int radius)
{
    // x = r*sin(θ) + centerX
    // y = r*cos(θ) + centerY
    // θ ∈ [0, 2pi]
    // θ = 2pi * i / 1000 = pi * i / 500, i ∈ [0, 1000]

    for ( int i = 0; i <= 1000; i++ )
    {
        double theta = pi * i / 500;
        int x = (int) radius * sin(theta) + centerX;
        int y = (int) radius * cos(theta) + centerY;
        SetPixel(hdc, x, y, RED);
    }
}

void DrawSolidCircle(const HDC hdc, const int centerX, const int centerY, const int radius)
{
    // θ ∈ [0, pi], ±y
    // θ = pi * i / 1000, 
    for ( int i = 0; i <= 1000; i++ )
    {
        double theta = pi * i / 1000;
        int width = (int) radius * sin(theta);
        int y = (int) radius * cos(theta) + centerY;
        for ( int x = centerX - width; x <= centerX + width; x++ )
        {
            SetPixel(hdc, x, y, RED);
        }
    }
}

// void DrawLineFromTo(const HDC hdc, const struct MyPoint * begin, const struct MyPoint * end)
void DrawLineFromTo(const HDC hdc, int beginX, int beginY, int endX, int endY)
{
    if ( beginX == endX )
    {
        if ( beginY > endY )
        {
            swap(&beginY, &endY);
        }

        // k (slope) does not exist
        for ( int y = beginY; y <= endY; y++ )
        {
            SetPixel(hdc, beginX, y, RED);
        }
    }
    else
    {
        if ( beginX > endX )
        {
            swap(&beginX, &endX);
            swap(&beginY, &endY);
        }

        double k = (endY - beginY) * 1.0 / (endX - beginX);
        // y = k * (x - x0) + y0
        for ( int x = beginX; x <= endX; x++ )
        {
            int y = (int)k * (x - beginX) + beginY;
            SetPixel(hdc, x, y, RED);
        }
    }
}

void DrawSolidSquare(const HDC hdc, const int ltX, int ltY, int rbX, int rbY)
{
    for ( int x = ltX; x <= rbX; x++ )
    {
        for ( int y = ltY; y <= rbY; y++ )
        {
            SetPixel(hdc, x, y, RED);
        }
    }
}

/*
void DrawSolidSquare(const HDC hdc, const POINT * ltPoint, const POINT * rbPoint)
{
    int ltX = ltPoint->x;
    int ltY = ltPoint->y;
    int rbX = rbPoint->x;
    int rbY = rbPoint->y;
    for ( int x = ltX; x <= rbX; x++ )
    {
        for ( int y = ltY; y <= rbY; y++ )
        {
            SetPixel(hdc, x, y, RED);
        }
    }
}
*/

void DrawCenterSolidSquare(const HDC hdc, const RECT * rect)
{
    int centerX = (rect->left + rect->right) / 2;
    int centerY = (rect->top + rect->bottom) / 2;
    int ltX = centerX - 50;
    int rbX = centerX + 50;
    int ltY = centerY - 50;
    int rbY = centerY + 50;
    DrawSolidSquare(hdc, ltX, ltY, rbX, rbY);
}

void DrawHi(HDC hdc)
{
    // H
    DrawLineFromTo(hdc, 200, 100, 200, 50);
    DrawLineFromTo(hdc, 200, 100, 200, 150);
    DrawLineFromTo(hdc, 200, 100, 250, 100);
    DrawLineFromTo(hdc, 250, 100, 250, 50);
    DrawLineFromTo(hdc, 250, 100, 250, 150);

    // Hi
    DrawLineFromTo(hdc, 285, 90, 270, 90);
    DrawLineFromTo(hdc, 285, 90, 285, 150);
    DrawLineFromTo(hdc, 285, 150, 270, 150);
    DrawLineFromTo(hdc, 285, 150, 300, 150);
    // DrawSolidCircle(hdc, 285, 75, 2);
    DrawSolidSquare(hdc, 283, 73, 287, 77);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);

            DrawDot(hdc, 30, 30);
            DrawLine(hdc);  // Done
            DrawCircle(hdc, 300, 300, 100);
            DrawSolidCircle(hdc, 50, 200, 30);
            DrawLineFromTo(hdc, 100, 100, 100, 50);     // Done
            DrawLineFromTo(hdc, 100, 100, 100, 150);    // Done
            DrawLineFromTo(hdc, 100, 100, 150, 150);    // Done
            DrawCenterSolidSquare(hdc, &rect);          // Done
            DrawHi(hdc);
            
            EndPaint(hwnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:    
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
}