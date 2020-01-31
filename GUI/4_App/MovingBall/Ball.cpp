#include <windows.h>
#include <math.h>
#include <time.h>

// gcc Ball.c -lgdi32 -mwindows -o Ball

#define PI 3.14
#define PI_DIV_2 1.57
#define FPS 40
#define RADIUS 32
#define SPEED 2
#define ID_TIMER1 1
#define ID_TIMER2 2
#define BLACK RGB(0,0,0)

class Ball
{
private:
    int __x;
    int __y;
    int __radius;
    int __speed;
    int __speedX;
    int __speedY;
    double __radian;

public:
    Ball(int x, int y, double degree)
    {
        __x = x;
        __y = y;
        __radius = RADIUS;
        __speed = SPEED;
        __radian = 2.0 * PI * degree / 360;
        __speedX = __speed * sin(__radian);
        __speedY = __speed * cos(__radian);
    }

    void setPosition(int x, int y)
    {
        __x = x;
        __y = y;
    }

    void setX(int x)
    {
        __x = x;
    }

    void setY(int y)
    {
        __y = y;
    }

    int getX()
    {
        return __x;
    }

    int getY()
    {
        return __y;
    }

    RECT getRect()
    {
        RECT rect;
        rect.left = __x - __radius;
        rect.right = __x + __radius;
        rect.top = __y - __radius;
        rect.bottom = __y + __radius;
        return rect;
    }

    void move(RECT & wall)
    {
        rebound(wall);
        __x += __speedX;
        __y += __speedY;
    }

    void rebound(RECT & wall)
    {
        if ( __x + __radius >= wall.right || __x - __radius <= wall.left )
        {
            __speedX = - __speedX;
        }
        if ( __y + __radius >= wall.bottom || __y - __radius <= wall.top )
        {
            __speedY = - __speedY;
        }
    }
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

void DrawBall(HDC hdc, Ball & ball)
{
    // DrawDot(hdc, ball.getX(), ball.getY());
    RECT rect = ball.getRect();
    // Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HGDIOBJ nullBrush = GetStockObject(NULL_BRUSH);
    HGDIOBJ originHGDIOBJ = SelectObject(hdc, nullBrush);
    PAINTSTRUCT ps;
    srand(time(NULL));
    static RECT rect;
    static Ball ball(320, 320, rand() % 360);
    switch (message)
    {
        case WM_CREATE:
            GetClientRect(hwnd, &rect);
            ball.setPosition(rect.right / 2, rect.bottom / 2);
            // move the ball
            SetTimer(hwnd, ID_TIMER1, 50, NULL);
            // refresh the frame
            SetTimer(hwnd, ID_TIMER2, 1000/FPS, NULL);
            return 0;
        case WM_TIMER:
            switch(wParam)
            {
                case ID_TIMER1:
                    ball.move(rect);    
                    break;
                case ID_TIMER2:
                    // ball.move(rect);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
            }
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);
            
            DrawBall(hdc, ball);

            EndPaint(hwnd, &ps);
            return 0;
        case WM_DESTROY:
            KillTimer(hwnd, ID_TIMER1);
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
}