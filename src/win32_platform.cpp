// #define SKB_DEBUG
#define DEBUG
#include "skb_debug.h"
#include "skb_types.h"

#define __UI__
#include "ui.h"

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")

// Win32:
static_global BITMAPINFO win32_globalBitmapinfo;
static_global HBITMAP win32_globalBitmap;

// Game:

// UI_BackBuffer global_UIBackBuffer;

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
        (void**)&global_UIBackBuffer.memory, 
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
        0, 0, global_UIBackBuffer.width, global_UIBackBuffer.height, 
        
        // Source (x, y, Width, Height)
        0, 0, global_UIBackBuffer.width, global_UIBackBuffer.height, 
        
        // Actual Pixe Memory,
        global_UIBackBuffer.memory,
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
        0, 0, global_UIBackBuffer.width, global_UIBackBuffer.height, 
        // Source (x, y, Width, Height)
        0, 0, global_UIBackBuffer.width, global_UIBackBuffer.height,
        // Actual Pixel Memory,
        global_UIBackBuffer.memory,
        // Description of that memory
        &win32_globalBitmapinfo,
        
        DIB_RGB_COLORS, 
        SRCCOPY
    );
    ReleaseDC(hwnd, hdc);
}

void win32_DrawText(
    const char* text,
    int x,
    int y,
    int width,
    int height
)
{
    // CreateCompatibleDC()
    // SelectObject()
    // SetBkMode();
    HDC hdc = CreateCompatibleDC(NULL);

    HBITMAP oldBitmap = (HBITMAP)SelectObject(
        hdc,
        win32_globalBitmap
    );

    // Don't draw a background behind the text.
    SetBkMode(hdc, TRANSPARENT);

    // Text color
    SetTextColor(hdc, RGB(255, 255, 255));

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
        DT_CENTER | DT_VCENTER | DT_SINGLELINE
    );

    SelectObject(hdc, oldBitmap);

    DeleteDC(hdc);
}

void win32_closeApp(void* handleWindow){
    global_UIMouseState.CLOSEBTN_CLICK = true;
    // if (global_UIMouseState.CLOSEBTN_CLICK)
        // DestroyWindow((HWND)handleWindow);
}

void win32_closeApp2(HWND hwnd) {
    DestroyWindow(hwnd);
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
            global_UIMouseState.LClick = true;
            global_UIMouseState.LClick_x = GET_X_LPARAM(lParam);
            global_UIMouseState.LClick_y = GET_Y_LPARAM(lParam);

            // [PASTE HERE]:
            
            return 0;
        }

        case WM_LBUTTONUP: {
            global_UIMouseState.LClick = false;
            global_UIMouseState.LClick_x = -1;
            global_UIMouseState.LClick_y = -1;

            if (global_UIMouseState.CLOSEBTN_CLICK) {
                global_UIMouseState.CLOSEBTN_CLICK = false;
                win32_closeApp2(hwnd);
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
            OutputDebugStringA("Window resized\n");
            return 0;
        }

        case WM_KEYDOWN: {
            // Detect ESC Key Press
            if (wParam == VK_ESCAPE) {
                DestroyWindow(hwnd);
            }

            return 0;
        }
    }
    
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

i32 func_colorChange(){
    i32 color = 0x00f7fdae;
    return color;
}

void func_moveAppOnTabBarClick(void* hwnd){
   // TODO: WILL NEED TO HAVE DS TO Store About TopBar Info 
            // Right now I know UPTO where the the TopBar() is..
            // [Checking]: Click On The TopBar
            // if (
            //     // x:
            //     (global_UIMouseState.LClick_x >= 0) &&
            //     (global_UIMouseState.LClick_x <= global_UIBackBuffer.width- 50) && // - 50, because of the width occupied by the close [X] button
            //     // y:
            //     (global_UIMouseState.LClick_y >= 0) && 
            //     (global_UIMouseState.LClick_y <= 50)  // 50: TopBar() Height
            // ){
            //     ReleaseCapture();
            //     SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            // }

    ReleaseCapture();
    SendMessage((HWND) hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class.
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "Learn DirectX11 Window Class";
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
    win32_create_backbuffer(clientAreaWidth, clientAreaHeight, &global_UIBackBuffer);

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
    i32 bgColor = 0x00aa00aa;
    UI_FillBackground(&global_UIBackBuffer, bgColor);
    // gameStateUpdate(
    //     &globalGameBackBuffer, 
    //     (-1),  // Intentionally Giving [INVALID VALUE]
    //     true);
    
    // Win32 Window
    ShowWindow(hwnd, nCmdShow);

    // Game-Loop:
    bool AppRunning = true;
    // For, FPS: (maybe?)
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency); // fixed. (during BOOT)
    i64 perfCountFrequency = frequency.QuadPart; // fixed
    
    LARGE_INTEGER startCounter;
    QueryPerformanceCounter(&startCounter);

    // RDTSC:
    i64 StartCPUCycleCount =  __rdtsc(); // the total number of CPU clock cycles that have elapsed since the processor was last reset or powered on

  
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
        
        // debugPrint("TimeElapsed: %.3llf ms\n", deltaTime*1000);
        // debugPrint("FPS: %.3llf\n", (double)perfCountFrequency / deltaTime);

        startCounter = endCounter; // Reset the start counter for the next frame
        
        // Update:
        // win32_keyboardInput(deltaTime, &globalGameBackBuffer);

        // [[ Render ]]:
        UI_FillBackground(&global_UIBackBuffer, bgColor);
        UI_TabBar(win32_closeApp,(void*) hwnd, func_moveAppOnTabBarClick);
        
        UI_Button(50, 300, 200, 40, NULL);
        UI_Button(100, 100, 100, 100, func_colorChange);


        // [[ PRESENT ]]:
        win32_DisplayUIBackBuffer(hwnd);
    }

    return 0;
}