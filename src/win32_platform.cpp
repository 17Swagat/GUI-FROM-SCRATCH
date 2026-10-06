/* TODO:
 * FIX:
 * BUG:
 * @LATER:
 * ASK:
 * NOTE:
 */

/*
[MAJOR TODOS]:
0. Multi Key input must be implemented. Right now only 1 key is able to be pressed at one time.
1. Making the button click smooth with animation
    - Now better than before.
    - Still Don't know how to have a bouncy animation on press
2. How to put a text and symbols to a block of memory and display it?
    - For Text in Buttons, Text Fields, etc..
    - Being able to load logos/symbols and display in the memory section.
3. [BUG]: Proper color changing on holding the title-bar (dragging it around).
*/

#define DEBUG
#include "skb_debug.h"
#include "skb_types.h"

#define __UI__
#include "ui.h"

#define __UILAYOUT__
#include "ui_layout.h"

#include <windows.h>
#include <windowsx.h> // Resposible for: (GET_X_LPARAM), (GET_Y_LPARAM)
#include <stdio.h>


#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")

// Win32:
static_global BITMAPINFO win32_globalBitmapinfo;
static_global HBITMAP win32_globalBitmap;
static_global HDC win32_global_TextDrawHDC = CreateCompatibleDC(NULL);
static_global HFONT win32_global_fonts[] = {
    CreateFontA(
        30, // font height
        0, // font width
        0, // angle
        0,
        FW_BOLD,      // weight
        FALSE,        // italic
        FALSE,        // underline
        FALSE,        // strikeout
        ANSI_CHARSET, // DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        "Consolas"
    ),
    CreateFontA(
        28, // font height
        0, // font width
        0, // angle
        0,
        FW_BOLD,      // weight
        FALSE,        // italic
        FALSE,        // underline
        FALSE,        // strikeout
        ANSI_CHARSET, // DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        "Consolas"
    )
};

void win32_create_backbuffer(int width, int height, UI_BackBuffer *gameBackBuffer)
{
    gameBackBuffer->width = width;
    gameBackBuffer->height = height;
    gameBackBuffer->memory = NULL;

    // [[ Bitmap Info ]]:=>
    win32_globalBitmapinfo.bmiHeader.biSize = sizeof(win32_globalBitmapinfo.bmiHeader);
    win32_globalBitmapinfo.bmiHeader.biWidth = width;
    win32_globalBitmapinfo.bmiHeader.biHeight = -height; // -ve = {top-down bitmap}
    win32_globalBitmapinfo.bmiHeader.biPlanes = 1;
    win32_globalBitmapinfo.bmiHeader.biBitCount = 32;
    win32_globalBitmapinfo.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(NULL);
    win32_globalBitmap = CreateDIBSection(
        hdc,
        &win32_globalBitmapinfo,
        DIB_RGB_COLORS,
        (void **)&global_UI_BackBuffer.memory,
        NULL,
        0);

    ReleaseDC(NULL, hdc);
}

// void win32_updateDIBDraw(HWND hwnd){
void win32_RepaintWindow(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    StretchDIBits(
        hdc,
        // Destination (x, y, Width, Height)
        0, 0, global_UI_BackBuffer.width, global_UI_BackBuffer.height,

        // Source (x, y, Width, Height)
        0, 0, global_UI_BackBuffer.width, global_UI_BackBuffer.height,

        // Actual Pixe Memory,
        global_UI_BackBuffer.memory,
        // Description of that memory
        &win32_globalBitmapinfo,
        DIB_RGB_COLORS,
        SRCCOPY);
    EndPaint(hwnd, &ps);
    // return 0;
}

void win32_DisplayUIBackBuffer(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    StretchDIBits(
        hdc,
        // Destination (x, y, Width, Height)
        0, 0, global_UI_BackBuffer.width, global_UI_BackBuffer.height,
        // Source (x, y, Width, Height)
        0, 0, global_UI_BackBuffer.width, global_UI_BackBuffer.height,
        // Actual Pixel Memory,
        global_UI_BackBuffer.memory,
        // Description of that memory
        &win32_globalBitmapinfo,

        DIB_RGB_COLORS,
        SRCCOPY);
    ReleaseDC(hwnd, hdc);
}

PLATFORM_Type_TEXTDIM PLATFORM_GET_TEXTDIMS(const char *text, i32 fontIndex)
{
    SIZE size = {};
    i32 len = lstrlenA(text);
    PLATFORM_Type_TEXTDIM textDims;
    // HFONT oldfont = (HFONT) SelectObject(win32_global_TextDrawHDC, win32_global_Font1);
    HFONT oldfont = (HFONT) SelectObject(win32_global_TextDrawHDC, win32_global_fonts[fontIndex]);

    if (GetTextExtentPoint32A(win32_global_TextDrawHDC, text, len, &size))
    {
        textDims.width = size.cx;
        textDims.height = size.cy;
    }
    else
    {
        // FAILED TO GET DIMENSIONS:
        textDims.width = -1;
        textDims.height = -1;
    }
    SelectObject(win32_global_TextDrawHDC, oldfont);
    return textDims;
}

void PLATFORM_IMPL_RenderTextToButton(
    // i32 btn_x, i32 btn_y, i32 btn_w, i32 btn_h, KeyText keytext
    i32 btn_x, i32 btn_y, i32 btn_w, i32 btn_h,
    i32 txt_width, i32 txt_height,
    const char *txt, i32 fontIndex)
{
    HDC hdc = win32_global_TextDrawHDC;//CreateCompatibleDC(NULL);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(hdc, win32_globalBitmap);
    // HFONT oldFont = (HFONT)SelectObject(hdc, win32_global_Font1);
    HFONT oldFont = (HFONT)SelectObject(hdc, win32_global_fonts[fontIndex]);

    SetBkMode(hdc, OPAQUE); // or OPAQUE if you want background
    SetTextColor(hdc, RGB(255, 0, 0));

    // Center txt inside the button
    int text_x = btn_x + (btn_w - txt_width) / 2;
    int text_y = btn_y + (btn_h - txt_height) / 2;
    // Draw (simple & reliable)
    TextOutA(hdc, text_x, text_y, txt, lstrlenA(txt));
    // RECT r = { text_x, text_y, text_x + dims.width, text_y + dims.height };
    // DrawTextA(hdc, txt, -1, &r, DT_LEFT | DT_SINGLELINE);
    // ASK: Is SelectObject() necessary in very func call?
    SelectObject(hdc, oldFont);
    SelectObject(hdc, oldBitmap);
    // NOTE: Deleting HDC in `WM_DESTROY` at once. To avoid creation of the HDC through CreateCompatibleDC(NULL) every time this function get's called.
    // DeleteDC(hdc);
}

void PLATFORM_INPUT_KEYBOARD(){
    u32 i = 0x01;
    const u32 max_k = 0xFE;

    while (i <= max_k) {
        if (GetAsyncKeyState(i) < 0) {
            // Key Down
            bool current = true;
            bool* press = &global_Input_KeyboardState.press[i];
            // bool* down = &global_Input_KeyboardState.down[i];
            *press = current && !(*press);
        }
        i++;
    }

    // if (GetAsyncKeyState('A') < 0){
    //     bool* press = &global_Input_KeyboardState.press['A'];
    //     bool* down = &global_Input_KeyboardState.down['D'];

    // }
    // if (GetAsyncKeyState('B') < 0){
    // }
    // if (GetAsyncKeyState('C') < 0){
    // }
}

// TODO: WORK on Keyboard Inputs:
void win32_KeyboardInput(double deltaTime)
{
    // static_local bool key_press=false;

    if (GetAsyncKeyState('A') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = 'A';
        // OutputDebugStringA("A\n");
        return;
    }
    if (GetAsyncKeyState('D') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = 'D';
        // OutputDebugStringA("D\n");
        return;
    }
    if (GetAsyncKeyState('W') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = 'W';
        // OutputDebugStringA("W\n");
        return;
    }

    // Number Inputs:
    if (GetAsyncKeyState('0') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '0';
    }
    else if (GetAsyncKeyState('1') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '1';
    }
    else if (GetAsyncKeyState('2') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '2';
    }
    else if (GetAsyncKeyState('3') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '3';
    }
    else if (GetAsyncKeyState('4') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '4';
    }
    else if (GetAsyncKeyState('5') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '5';
    }
    else if (GetAsyncKeyState('6') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '6';
    }
    else if (GetAsyncKeyState('7') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '7';
    }
    else if (GetAsyncKeyState('8') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '8';
    }
    else if (GetAsyncKeyState('9') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '9';
    }
    else if (GetAsyncKeyState('10') < 0)
    {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '10';
    }
    else
    {
        global_UI_KeyboardState.pressed = false;
        global_UI_KeyboardState.key = '\0';
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        // Mouse Click on Button:
        case WM_LBUTTONDOWN:
        {
            global_UI_MouseState.LClick = true;
            global_UI_MouseState.LClick_x = GET_X_LPARAM(lParam);
            global_UI_MouseState.LClick_y = GET_Y_LPARAM(lParam);

            // [TABBAR CLICK(Dragging)]:=>
            if (UIFunc_isMouseOver_TopBar())
            {
                global_UI_Topbar.pressed = true;
                global_UI_Topbar.draggingWindow = true;

                POINT mouse;
                GetCursorPos(&mouse);

                // g_dragStartMouse = mouse;
                // global_UI_Topbar.mousePoint = mouse;
                global_UI_Topbar.mousePoint.x = mouse.x;
                global_UI_Topbar.mousePoint.y = mouse.y;

                RECT windowRect;
                GetWindowRect(hwnd, &windowRect);

                global_UI_Topbar.windowStart.x = windowRect.left;
                global_UI_Topbar.windowStart.y = windowRect.top;
                // g_windowStart.x = windowRect.left;
                // g_windowStart.y = windowRect.top;

                InvalidateRect(hwnd, NULL, FALSE);

                SetCapture(hwnd);
            }
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            // if (g_draggingWindow)
            if (global_UI_Topbar.draggingWindow)
            {
                POINT mouse;
                GetCursorPos(&mouse);

                // int dx = mouse.x - g_dragStartMouse.x;
                // int dy = mouse.y - g_dragStartMouse.y;
                int dx = mouse.x - global_UI_Topbar.mousePoint.x;
                int dy = mouse.y - global_UI_Topbar.mousePoint.y;

                SetWindowPos(
                    hwnd,
                    NULL,
                    global_UI_Topbar.windowStart.x + dx,
                    global_UI_Topbar.windowStart.y + dy,
                    0,
                    0,
                    SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            }

            return 0;
        }

        case WM_LBUTTONUP:
        {
            global_UI_MouseState.LClick = false;
            global_UI_MouseState.LClick_x = -1;
            global_UI_MouseState.LClick_y = -1;

            // TopBar:
            if (global_UI_Topbar.draggingWindow)
            {
                global_UI_Topbar.draggingWindow = false;
                global_UI_Topbar.pressed = false;
                ReleaseCapture();
                InvalidateRect(hwnd, NULL, FALSE);
            }

            // Close Button [X]:
            if (global_UI_MouseState.CLOSEBTN_CLICK)
            {
                global_UI_MouseState.CLOSEBTN_CLICK = false;

                // Check: if the cursor is on the Cross Btn of not?
                global_UI_MouseState.LClick_x = GET_X_LPARAM(lParam);
                global_UI_MouseState.LClick_y = GET_Y_LPARAM(lParam);
                if (UIFunc_isCursorOnCloseBtn())
                    DestroyWindow(hwnd);
            }

            return 0;
        }

        case WM_PAINT:
        {
            win32_RepaintWindow(hwnd);
            return 0;
        }

        case WM_DESTROY:
        {
            // Clearing DCs:
            DeleteDC(win32_global_TextDrawHDC);

            // TODO: State State Saving:
            PostQuitMessage(0); // What does this function do? It posts a quit message to the message queue, signaling the application to terminate.
            return 0;
        }

            // NOT Usefull now: [Since implementing my own [X] btn nd TabBar]
            // case WM_CLOSE: {
            // THIS CODE GET'S ACTIVATED: "When user clicks the X button TO CLOSE THE WINDOW".
            // DestroyWindow(hwnd);
            // return 0;
            // }

            // NOT in use right now. Since disabled window resizing
        case WM_SIZE:
        {
            // Handle window resizing if needed
            // OutputDebugStringA("Window resized\n");
            return 0;
        }

        case WM_KEYDOWN:
        {
            // if (wParam == 'A') {
            //     OutputDebugStringA("A is pressed\n");
            // }

            // Detect ESC Key Press
            if (wParam == VK_ESCAPE)
            {
                #if defined(DEBUG)
                    DestroyWindow(hwnd);
                #endif
            }
            return 0;
        }
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Font Init : (Done in Globals) // win32_InitText();

    // Register the window class.
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "GUI FROM SCRATCH";
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    RegisterClassA(&wc);

    // Drawing Area:
    i32 clientAreaWidth = 1000;
    i32 clientAreaHeight = 800;

    RECT rect = {};
    rect.right = clientAreaWidth;
    rect.bottom = clientAreaHeight;

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    int windowAreaWidth = rect.right - rect.left;
    int windowAreaHeight = rect.bottom - rect.top;

    // Positioning the window in the center of the screen:
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // Get the width of the screen
    int screenHeight = GetSystemMetrics(SM_CYSCREEN); // Get the height of the screen
    int windowPosX = (screenWidth - windowAreaWidth) / 2;
    int windowPoxY = (screenHeight - windowAreaHeight) / 2;

    // BackBuffer Creation (Win32):
    win32_create_backbuffer(clientAreaWidth, clientAreaHeight, &global_UI_BackBuffer);

    // Making window Non-Resizable.
    DWORD windowStyle = (WS_OVERLAPPED | WS_POPUP
                         // |  WS_MINIMIZEBOX |  WS_CAPTION | WS_SYSMENU
    );
    HWND hwnd = CreateWindowA(
        wc.lpszClassName,  // Window class
        "UI From Scratch", // Window text
        windowStyle,       // WS_OVERLAPPEDWINDOW,               // Window style

        // Window Position and Size
        // (x, y) coords of the Top-Left Corner of the Window
        windowPosX, windowPoxY, // CW_USEDEFAULT, CW_USEDEFAULT,
        // (WIDTH, HEIGHT)
        windowAreaWidth, windowAreaHeight, // CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,                              // Parent window
        NULL,                              // Menu
        hInstance,                         // Instance handle
        NULL                               // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // Game Initialization:
    // "Filling Backbuffer with BG color & RECT color"
    // i32 bgColor = 0x00aaaaaa;
    // UI_FillBackground(&global_UI_BackBuffer, bgColor);

    // Win32 Window
    ShowWindow(hwnd, nCmdShow);

    // Game-Loop:
    bool AppRunning = true;

    // // For, FPS: (maybe?)
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);       // fixed. (during BOOT)
    i64 perfCountFrequency = frequency.QuadPart; // fixed

    LARGE_INTEGER startCounter;
    QueryPerformanceCounter(&startCounter);

    // // RDTSC:
    // i64 StartCPUCycleCount =  __rdtsc(); // the total number of CPU clock cycles that have elapsed since the processor was last reset or powered on
    while (AppRunning)
    {
        MSG msg = {};

        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                AppRunning = false;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        if (!AppRunning)
            break;

        // 2. Measure elapsed time
        LARGE_INTEGER endCounter;
        QueryPerformanceCounter(&endCounter);

        // i64 EndCPUCycleCount =  __rdtsc();

        // i64 elapsedCPUCycleCount = EndCPUCycleCount - StartCPUCycleCount;
        // StartCPUCycleCount = EndCPUCycleCount;
        // debugPrint("Elapsed CPU Cycles: %lld\n", elapsedCPUCycleCount);

        // debugPrint("Start-Counter = %lld\n", startCounter);
        // debugPrint("End-Counter = %lld\nn", endCounter);

        /*
            [QueryPerformanceCounter] diff:   x counts elapsed
            [QueryPerformanceFrequency]: y counts/sec
            [Seconds]:                  (x counts) / (y counts/sec) = (x/y) secs elapsed.
        */

        i64 counterElapsed = endCounter.QuadPart - startCounter.QuadPart;
        // deltaTime = [SECONDS ELAPSED between the 2 counters]
        double deltaTime =
            (double)counterElapsed / (double)perfCountFrequency;

        // // debugPrint("TimeElapsed: %.3llf ms\n", deltaTime*1000);
        // // debugPrint("FPS: %.3llf\n", (double)perfCountFrequency / deltaTime);
        double targetFrameTime = 1.0 / 60.0;
        // if (frameTime < targetFrameTime)
        if (deltaTime < targetFrameTime)
        {
            DWORD sleepMS =
                (DWORD)((targetFrameTime - deltaTime) * 1000.0);
            //    (DWORD)((targetFrameTime - frameTime) * 1000.0);

            Sleep(sleepMS);
        }

        // [[ Render ]]:
        // win32_KeyboardInput(deltaTime);
        PLATFORM_INPUT_KEYBOARD();
        UI_LAYOUT(hwnd, deltaTime);

        startCounter = endCounter; // Reset the start counter for the next frame

        // [[ PRESENT ]]:
        win32_DisplayUIBackBuffer(hwnd);

        // Sleep(5);
    }

    return 0;
}