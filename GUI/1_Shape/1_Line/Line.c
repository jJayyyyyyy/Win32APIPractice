#include <windows.h>

#define RED RGB(255,0,0)
#define GREEN RGB(0,255,0)
#define BLACK RGB(0,0,255)

// gcc template.c -lgdi32 -mwindows -o template

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
        TEXT("Line"),
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

void DrawLine(HDC hdc, int beginX, int beginY, int endX, int endY, COLORREF color)
{
    // set Pen-Style
    HPEN hPen = CreatePen(PS_SOLID, 1, color);
    HPEN hOldPen = SelectObject(hdc, hPen);
    MoveToEx(hdc, beginX, beginY, NULL);
    LineTo(hdc, endX, endY);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    POINT currentPosition;
    GetCurrentPositionEx(hdc, &currentPosition);
    MoveToEx(hdc, currentPosition.x + 100, currentPosition.y, NULL);
    LineTo(hdc, 200, 100);
}

void Gridding(HDC hdc, const RECT * rect, int spacing)
{
    int top = rect->top;
    int bottom = rect->bottom;
    int left = rect->left;
    int right = rect->right;

    // vertical
    for ( int y = top; y < bottom; y += spacing)
    {
        MoveToEx(hdc, left, y, NULL);
        LineTo(hdc, right, y);
    }

    // horizontal
    for ( int x = left; x < right; x += spacing)
    {
        MoveToEx(hdc, x, top, NULL);
        LineTo(hdc, x, bottom);
    }
}

void DrawStar(HDC hdc)
{
    POINT p1, p2, p3, p4, p5;
    p1.x = 30;
    p1.y = 10;

    p2.x = 20;
    p2.y = 50;
    
    p3.x = 50;
    p3.y = 20;

    p4.x = 10;
    p4.y = 20;

    p5.x = 40;
    p5.y = 50;

    MoveToEx(hdc, p1.x, p1.y, NULL);
    LineTo(hdc, p2.x, p2.y);
    LineTo(hdc, p3.x, p3.y);
    LineTo(hdc, p4.x, p4.y);
    LineTo(hdc, p5.x, p5.y);
    LineTo(hdc, p1.x, p1.y);

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
            DrawLine(hdc, 100, 100, 200, 200, RED);
            Gridding(hdc, &rect, 100);
            DrawStar(hdc);
            EndPaint(hwnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
}