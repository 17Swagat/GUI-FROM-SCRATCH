/*
[MAJOR TODOS]:
0. Right now only 1 click is possible. If more than 1 key press, does not take place.
1. Making the button click smooth with animation 
    - Now better than before.
    - Still Don't know how to have a bouncy animation on press
2. How to put a text and symbols to a block of memory and display it?
    - For Text in Buttons, Text Fields, etc..
    - Being able to load logos/symbols and display in the memory section.
3. [BUG]: Proper color changing on holding the title-bar (dragging it around).
*/

// #define SKB_DEBUG
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


void win32_create_backbuffer(int width, int height, UI_BackBuffer* gameBackBuffer) 
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
        (void**)&global_UI_BackBuffer.memory, 
        NULL, 
        0
    );

    ReleaseDC(NULL, hdc);
}

// void win32_updateDIBDraw(HWND hwnd){
void win32_RepaintWindow(HWND hwnd){
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
        SRCCOPY
    );
    EndPaint(hwnd, &ps);
    // return 0;
}

void win32_DisplayUIBackBuffer(HWND hwnd) {
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
        SRCCOPY
    );
    ReleaseDC(hwnd, hdc);
}

HFONT win32_font;
void win32_InitText()
{
    win32_font = CreateFontA(
        16,                     // font height
        0,                      // font width
        0,                      // angle
        0,
        FW_NORMAL,              // weight
        FALSE,                  // italic
        FALSE,                  // underline
        FALSE,                  // strikeout
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        "Consolas"
    );
}

void win32_DrawText(
    const char* text,
    int x,
    int y,
    int width,
    int height
)
{
    HDC hdc = CreateCompatibleDC(NULL);

    HBITMAP oldBitmap = (HBITMAP)SelectObject(
        hdc,
        win32_globalBitmap
    );

    HFONT oldFont = (HFONT)SelectObject(
        hdc,
        win32_font
    );

    SetBkMode(hdc, TRANSPARENT);

    SetTextColor(
        hdc,
        RGB(255, 255, 255)
    );

    RECT rect = {};
    rect.left   = x;
    rect.top    = y;
    rect.right  = x + width;
    rect.bottom = y + height;

    DrawTextA(
        hdc,
        text,
        -1,
        &rect,
        DT_CENTER |
        DT_VCENTER |
        DT_SINGLELINE
    );

    SelectObject(hdc, oldFont);
    SelectObject(hdc, oldBitmap);

    DeleteDC(hdc);
}

// void win32_DrawText(
//     const char* text,
//     int x,
//     int y,
//     int width,
//     int height
// )
// {

//     // New:
//     HFONT font = CreateFontA(
//         32,                     // height
//         0,                      // width
//         0,                      // escapement
//         0,                      // orientation
//         FW_NORMAL,              // weight
//         FALSE,                  // italic
//         FALSE,                  // underline
//         FALSE,                  // strikeout
//         DEFAULT_CHARSET,
//         OUT_DEFAULT_PRECIS,
//         CLIP_DEFAULT_PRECIS,
//         ANTIALIASED_QUALITY,
//         DEFAULT_PITCH | FF_DONTCARE,
//         "Consolas"
//     );


//     // Old:
//     // CreateCompatibleDC()
//     // SelectObject()
//     // SetBkMode();
//     HDC hdc = CreateCompatibleDC(NULL);

//     HBITMAP oldBitmap = (HBITMAP)SelectObject(
//         hdc,
//         win32_globalBitmap
//     );

//     // Don't draw a background behind the text.
//     // SetBkMode(hdc, RGB(0, 255, 255));
//     SetBkMode(hdc, TRANSPARENT);

//     // Text color
//     SetTextColor(hdc, RGB(255, 255, 255));

//     RECT rect = {};
//     rect.left   = x;
//     rect.top    = y;
//     rect.right  = x + width;
//     rect.bottom = y + height;

//     DrawTextA(
//         hdc,
//         text,
//         -1,
//         &rect,
//         DT_CENTER | DT_VCENTER | DT_SINGLELINE
//     );

//     SelectObject(hdc, oldBitmap);

//     DeleteDC(hdc);
// }

void win32_closeApp(void* handleWindow){
    global_UI_MouseState.CLOSEBTN_CLICK = true;
}


// struct KeyState {
//     bool isDown;
//     bool released;
//     bool pressed;
// };

// KeyState Key_A;
// KeyState Key_W;

// void win32_updateKey(KeyState* key, i32 vk){
//     // TODO: Study about GetAsyncKeyState(vk)
//     bool currentlyDown = (GetAsyncKeyState(vk) & 0x8000) != 0;
//     key->pressed = currentlyDown && !key->isDown;
//     key->released = !currentlyDown && key->isDown;
//     key->isDown = currentlyDown;
// }

// TODO: WORK on Keyboard Inputs:
void win32_KeyboardInput(double deltaTime)
{
    if (GetAsyncKeyState('A') < 0 ) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = 'A';
        // OutputDebugStringA("A\n");
        return;
    }  
    if (GetAsyncKeyState('D') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = 'D';
        // OutputDebugStringA("D\n");
        return;
    } 
    if (GetAsyncKeyState('W') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = 'W';
        // OutputDebugStringA("W\n");
        return;
    } 
    if (GetAsyncKeyState('0') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '0';
        return;
    }
    if (GetAsyncKeyState('1') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '1';
        return;
    }
    if (GetAsyncKeyState('2') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '2';
        return;
    }
    if (GetAsyncKeyState('3') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '3';
        return;
    }
    if (GetAsyncKeyState('4') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '4';
        return;
    }
    if (GetAsyncKeyState('5') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '5';
        return;
    }
    if (GetAsyncKeyState('6') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '6';
        return;
    }
    if (GetAsyncKeyState('7') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '7';
        return;
    }
    if (GetAsyncKeyState('8') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '8';
        return;
    }
    if (GetAsyncKeyState('9') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '9';
        return;
    }
    if (GetAsyncKeyState('10') < 0) {
        global_UI_KeyboardState.pressed = true;
        global_UI_KeyboardState.key = '10';
        return;
    }
    
    global_UI_KeyboardState.pressed = false;
    global_UI_KeyboardState.key = '\0';
}



void PLATFORM_IMPL_DrawText(const char* text, int x, int y, int width, int height){
    win32_DrawText(text, x, y, width, height);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        // Mouse Click on Button:
        case WM_LBUTTONDOWN: {
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
                    SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
                );
            }
        
            return 0;
        }

        case WM_LBUTTONUP: {
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
            if (global_UI_MouseState.CLOSEBTN_CLICK) {
                global_UI_MouseState.CLOSEBTN_CLICK = false;

                // Check: if the cursor is on the Cross Btn of not?
                global_UI_MouseState.LClick_x = GET_X_LPARAM(lParam);
                global_UI_MouseState.LClick_y = GET_Y_LPARAM(lParam);
                if (UIFunc_isCursorOnCloseBtn())
                    DestroyWindow(hwnd);
            }

            return 0;
        }

        case WM_PAINT: {
                win32_RepaintWindow(hwnd);
                return 0;
        }
 

        case WM_DESTROY: {
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
        case WM_SIZE: {
            // Handle window resizing if needed
            // OutputDebugStringA("Window resized\n");
            return 0;
        }

        case WM_KEYDOWN: {
            // if (wParam == 'A') {
            //     OutputDebugStringA("A is pressed\n");
            // }
            
            // Detect ESC Key Press
            if (wParam == VK_ESCAPE) {
                #if defined(DEBUG)
                    DestroyWindow(hwnd);
                #endif
            }
            return 0;
        }

        // case WM_KEYUP: {
        //     // if (wParam == 'A') {
        //     //     OutputDebugStringA("A is released\n");
        //     // }
            
        //     return 0;
        // }

        // UNDO COMMENT:
        // case WM_NCLBUTTONDOWN: {
        //     if (global_UI_MouseState.TABBAR_CLICK) {
        //         global_UI_Topbar.color = COLOR_TABBAR_CLICKED;
        //         // global_UI_MouseState.TABBAR_COLOR = COLOR_TABBAR_CLICKED;
        //     }
            
        //     return DefWindowProcA(hwnd, uMsg, wParam, lParam);
        //     // return 0;
        // }

        // case WM_NCLBUTTONUP: {
        //     if (global_UI_MouseState.TABBAR_CLICK)
        //         global_UI_MouseState.TABBAR_CLICK = false;
        //     // global_UI_MouseState.TABBAR_COLOR = COLOR_TABBAR_DEFAULT;
        //     global_UI_Topbar.color = COLOR_TABBAR_DEFAULT;
        //     win32_RepaintWindow(hwnd);
        //     return DefWindowProcA(hwnd, uMsg, wParam, lParam);
        // }
    }
    
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}


void win32_moveAppOnTabBarClick(void* hwnd){
    // #NEW:
    // Will handle window drag myself
    
    // #OLD:
    // // TODO: WILL NEED TO HAVE DS TO Store About TopBar Info 
    // // global_UI_MouseState.TABBAR_CLICK = true;
    // // What is the use of ReleaseCapture()? Even without it the program is behaving as expected?
    // // ReleaseCapture();
    ///////////////
    // SendMessage((HWND) hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    ///////////////
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Font Init
    win32_InitText();

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
    int screenWidth = GetSystemMetrics(SM_CXSCREEN); // Get the width of the screen
    int screenHeight = GetSystemMetrics(SM_CYSCREEN); // Get the height of the screen
    int windowPosX = (screenWidth - windowAreaWidth) / 2;
    int windowPoxY = (screenHeight - windowAreaHeight) / 2;

    // BackBuffer Creation (Win32):
    win32_create_backbuffer(clientAreaWidth, clientAreaHeight, &global_UI_BackBuffer);

    // Making window Non-Resizable.
    DWORD windowStyle = (
        WS_OVERLAPPED | WS_POPUP 
        // |  WS_MINIMIZEBOX |  WS_CAPTION | WS_SYSMENU 
    ); 
    HWND hwnd = CreateWindowA(
        wc.lpszClassName, // Window class
        "UI From Scratch",          // Window text
        windowStyle, // WS_OVERLAPPEDWINDOW,               // Window style

        // Window Position and Size
        // (x, y) coords of the Top-Left Corner of the Window
        windowPosX, windowPoxY,// CW_USEDEFAULT, CW_USEDEFAULT, 
        // (WIDTH, HEIGHT)
        windowAreaWidth, windowAreaHeight, // CW_USEDEFAULT, CW_USEDEFAULT, 
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
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
    QueryPerformanceFrequency(&frequency); // fixed. (during BOOT)
    i64 perfCountFrequency = frequency.QuadPart; // fixed
    
    LARGE_INTEGER startCounter;
    QueryPerformanceCounter(&startCounter);

    // // RDTSC:
    // i64 StartCPUCycleCount =  __rdtsc(); // the total number of CPU clock cycles that have elapsed since the processor was last reset or powered on
    while (AppRunning) {
        MSG msg = { };
        
        while(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)){
            if (msg.message == WM_QUIT) {
                AppRunning = false;
                break;
            }
            
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        if(!AppRunning)
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
            (double) counterElapsed / (double) perfCountFrequency; 
        
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
        win32_KeyboardInput(deltaTime);
        UI_LAYOUT(hwnd, deltaTime, win32_moveAppOnTabBarClick);

        startCounter = endCounter; // Reset the start counter for the next frame

        // [[ PRESENT ]]:
        win32_DisplayUIBackBuffer(hwnd);

        // Sleep(5);
    }

    return 0;
}