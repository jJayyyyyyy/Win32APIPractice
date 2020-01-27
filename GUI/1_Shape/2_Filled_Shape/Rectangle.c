#include <windows.h>

// gcc template.c -lgdi32 -mwindows -o template

/*
API:

*   [Rectangle](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-rectangle)

    BOOL Rectangle(
        [in] HDC hdc,
        [in] int left,
        [in] int top,
        [in] int right,
        [in] int bottom
    );

*   [FrameRect](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-framerect)

    int FrameRect(
        [in] HDC        hDC,
        [in] const RECT *lprc,
        [in] HBRUSH     hbr
    );

*   [GetStockObject](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getstockobject)

*/

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

void TestRect(HDC hdc)
{
    Rectangle(hdc, 50, 50, 150, 150);
    LineTo(hdc, 300, 300);
    RECT rect;
    rect.left = 100;
    rect.top = 100;
    rect.right = 200;
    rect.bottom = 200;
    HBRUSH brush = CreateSolidBrush(RGB(255,0,0));
    FrameRect(hdc, &rect, brush);
    
    rect.left = 120;
    rect.top = 120;
    rect.right = 220;
    rect.bottom = 220;
    brush = GetStockObject(BLACK_BRUSH);
    FrameRect(hdc, &rect, brush);

    Rectangle(hdc, 180, 180, 280, 280);
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
            
            TestRect(hdc);

            EndPaint(hwnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
}