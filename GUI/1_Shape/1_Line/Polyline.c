#include <windows.h>

#define RED RGB(255,0,0)
#define GREEN RGB(0,255,0)
#define BLACK RGB(0,0,255)

// gcc template.c -lgdi32 -mwindows -o template
/*
API:

*   [Polyline](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-polyline). The Polyline function draws a series of line segments by connecting the points in the specified array.

    API:

    BOOL Polyline(HDC hdc, const POINT *apt, int cpt);

    *   Parameters

        HDC hdc, A handle to a device context.

        POINT *apt, A pointer to an array of POINT structures, in logical units.

        int cpt, The number of points in the array. This number must be greater than or equal to two.


*   [PolylineTo](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-polylineTo)

    API:

    BOOL PolylineTo(HDC hdc, const POINT *apt, DWORD cpt);

*   [PolyPolyline](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-PolyPolyline)

    API:

    BOOL PolyPolyline(HDC hdc, const POINT *apt, const DWORD *asz, DWORD csz);

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
        TEXT("PolyLine"),
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

void TestPolyline(HDC hdc)
{
    int cpt = 3;
    POINT apt[] = {100,100, 200,200, 300,100};
    Polyline(hdc, apt, cpt); 
}

void TestPolylineTo(HDC hdc)
{
    int cpt = 3;
    POINT apt[] = {100,200, 200,300, 300,200};
    // Unlike Polyline(), the PolylineTo() function firstly draw a line
    //     from origin point (0, 0) to the first point of apt[]. After 
    //     that, it behaves like Polyline().
    PolylineTo(hdc, apt, cpt);
}

void TestPolyPolyline(HDC hdc)
{
    POINT apt[] = { 100,100, 200,200, // 2pts
                    300,100, 400,200, 500,100, 600,200, // 4pts
                    700,100, 800,200, 900,100}; // 3pts
    DWORD asz[] = {2, 4, 3};    // There are 2/4/3 POINTs in each line
    int csz = 3;    // There are [csz] groups of lines
    PolyPolyline(hdc, apt, asz, csz);
}

void DrawSine(HDC hdc, RECT * rect)
{

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
            
            // TestPolyline(hdc);
            // TestPolylineTo(hdc);
            TestPolyPolyline(hdc);
            DrawSine(hdc, &rect);

            EndPaint(hwnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
}