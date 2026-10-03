#pragma once

#include "skb_types.h"

#define COLOR_BG_COLOR 0x00440022
#define COLOR_TABBAR_DEFAULT 0x00323233
#define COLOR_TABBAR_CLICKED 0x00543233
#define COLOR_BUTTON_DEFAULT 0x000000ff
#define COLOR_BUTTON_CLICKED 0x00aa00a0

struct UI_BackBuffer{
    i32 width;
    i32 height;
    void* memory; 
    const i32 bytesPerPixel = 4; 
};


// typedef struct tagPOINT
// {
//     long  x;
//     long  y;
// } MOUSEPOINT;

struct UI_Topbar {
    i32 x;
    i32 y;
    u32 width;
    u32 height;
    u32 color;
    bool pressed = false;
    bool draggingWindow = false;
    struct {long x; long y;} mousePoint;
    struct {long x; long y;} windowStart;
    // MOUSEPOINT mousePoint;
    // MOUSEPOINT windowStart;
};

struct UI_MouseState {
    bool LClick = false;
    bool RClick = false;
    i32 LClick_x = -1; 
    i32 LClick_y = -1; 

    // TODO: SEPRATE THE TAB SPECIFIC INFORMATION INTO A DIFFERENT STRUCT:
    // TabBar
    bool TABBAR_CLICK = false;
    // Will Get Filled:
    // i32 TABBAR_X = 0;
    // i32 TABBAR_Y = 0;
    // u32 TABBAR_WIDTH = 0;
    // u32 TABBAR_HEIGHT = 50; 
    // u32 TABBAR_COLOR = COLOR_TABBAR_DEFAULT;

    // CLOSE [X] Btn:
    bool CLOSEBTN_CLICK = false;
    // Will Get Filled:
    i32 CLOSEBTN_POS_X = 0;
    i32 CLOSEBTN_POS_Y = 0;
    u32 CLOSEBTN_WIDTH = 0;
    u32 CLOSEBTN_HEIGHT = 0;
};


// struct UI_TabBarState{
//     inline static u32 color = 0x00323233;
// };

struct UI_KeyboardState{
    bool pressed = false;
    char key = '\0';
};

UI_BackBuffer global_UI_BackBuffer;
UI_MouseState global_UI_MouseState;
UI_KeyboardState global_UI_KeyboardState;
UI_Topbar global_UI_Topbar;

// TODO: Think about this later
// static_global bool GUI_INIT;

void UI_FillBackground(UI_BackBuffer *backbuffer, i32 color);
void UI_Button(
    i32 x, i32 y, i32 width, i32 height, i32 (*func)()
);
void UI_ButtonClose(i32 x, i32 y, i32 width, i32 height);
void UI_TabBar(
    void (*closeApp)(void*), 
    void* handleWindow,
    void (*tabbarClick)(void*)
    // ,i32 height
);


// bool UIFunc_isTabBarClick();
bool UIFunc_isMouseOver_TopBar();
bool UIFunc_isCursorOnCloseBtn();

// Will Get Implemented by the Platform Layer:
void PLATFORM_IMPL_DrawText(const char* text, i32 x, i32 y, i32 width, i32 height);


/***************************************************** */
// Implementation:=>
/***************************************************** */
struct ButtonPressed {
    i32 x, y;
    // u32 width, height;
    i32 width, height;
    u32 clickColor = COLOR_BUTTON_CLICKED;
    bool pressed = false;
    char triggerKey = '\0';
    double scale = 1.0;
};

void UI_ButtonPressed(ButtonPressed* button, double deltaTime);

#ifdef __UI__

// TEMP: Experimenting: Trying to simulate a button press:
// void ButtonPress(int x, int y, int width, int height){
// }

// struct ButtonPressed {
//     i32 x, y;
//     u32 width, height;
//     u32 clickColor = COLOR_BUTTON_CLICKED;
//     bool pressed = false;
// };

void UI_ButtonPressed(
    ButtonPressed* button,
    double deltaTime
){
    // static_local int x = button->x;
    // static_local int y = button->y;
    // static_local int width = button->width;
    // static_local int height = button->height;
    int x = button->x;
    int y = button->y;
    int width = button->width;
    int height = button->height;
    i32 color = 0x00555555;

    double buttonScale = 1.0;
    double shrinkSpeed = 0.0005; // 5.0f
    // double calc = (speed * deltaTime); 

    // Keyboard Click: 
    // Current 'A'/'D' Trigger
    bool triggerButton = false;
    if (global_UI_KeyboardState.pressed){
        if (global_UI_KeyboardState.key == button->triggerKey) {
            triggerButton = true;
        }
        // if (
        //     (global_UI_KeyboardState.key == 'A') ||
        //     (global_UI_KeyboardState.key == 'D')
        // ) {
        //     triggerButton = true;
        // }
    }
    
    // Detecting Mouse Click:
    if (global_UI_MouseState.LClick) {
        if (
            (global_UI_MouseState.LClick_x >= x) && 
            (global_UI_MouseState.LClick_x <= x+ width) &&
            (global_UI_MouseState.LClick_y >= y) &&
            (global_UI_MouseState.LClick_y <= y + height)
        ){
            button->pressed = true;
            // button->clickColor = COLOR_BUTTON_CLICKED;
            button->clickColor = color;
            color = button->clickColor;
        }
    }

    // Animate:
    // if (button->pressed) {
    //     button->scale -= shrinkSpeed * deltaTime;

    //     if (button->scale < 0.85)
    //         button->scale = 0.85;
    // }
    // else {
    //     button->scale += shrinkSpeed * deltaTime;

    //     if (button->scale > 1.0)
    //         button->scale = 1.0;
    // }
    if (button->pressed || triggerButton) {
        // button->scale -=  shrinkSpeed * deltaTime;
        // if (button->scale < 0.85)
        //     button->scale = 0.85;

        // double rate = 0.9 * deltaTime * 0.0000001;//* (shrinkSpeed * deltaTime);
        // width -= rate;
        // height -= rate;
        // x += rate;
        // y += rate;

        if (width <= button->width * 0.90)
            width = button->width * 0.90;
        if (height <= button->height * 0.90)
            height = button->height * 0.90;
        if (x <= button->x + (button->width - width)/2)
            x = button->x + (button->width - width)/2;
        if (y <= button->y + (button->height - height)/2)
            y = button->y + (button->height - height)/2;

        width = button->width * 0.90;
        height = button->height * 0.90;
        x = button->x + (button->width - width)/2;
        y = button->y + (button->height - height)/2;
    } else {
        width = button->width;
        height = button->height;
        x = button->x;
        y = button->y;
    }

    // Calculate rendered rectangle
    // width = button->width * button->scale;
    // height = button->height * button->scale;

    // x = button->x + (button->width - width) / 2;
    // y = button->y + (button->height - height) / 2;

    // button->x = x;
    // button->y = y;
    // button->width = width;
    // button->height = height;


    u32* pixel =  (
        (u32*)global_UI_BackBuffer.memory + y * global_UI_BackBuffer.width + x
    );
    
    for(i32 y = 0; y < height; y++) {
        for(i32 i = 0; i < width; i++){
            if (
                (y < global_UI_BackBuffer.height - height) 

                    || 
                (y > 0)
            )
                *(pixel+i) = color;
        }
        if ((y < global_UI_BackBuffer.height - height) || (y > 0))
            pixel += global_UI_BackBuffer.width;
    }

    PLATFORM_IMPL_DrawText(&button->triggerKey, x, y, 10, 10);
}

void UI_FillBackground(UI_BackBuffer *backbuffer, i32 color){
    for(i32 i = 0; i < backbuffer->width * backbuffer->height; i++) {
        ((i32*)backbuffer->memory)[i] = color;
    }
}

void UI_TabBar(
    void (*closeApp)(void*), 
    void* handleWindow,
    void (*tabbarClick)(void*)
    // ,i32 height = 50
) {
    i32 width = global_UI_BackBuffer.width;
    u32* pixel = (u32*)global_UI_BackBuffer.memory;
    // u32 color = global_UI_MouseState.TABBAR_COLOR;
    // if (height != 50) {
    //     global_UI_MouseState.TABBAR_HEIGHT = height;
    // }

    if (global_UI_MouseState.LClick) {
        //    if (
        //        (global_UI_MouseState.LClick_x >= 0) && 
        //        (global_UI_MouseState.LClick_x <= global_UI_BackBuffer.width - 50) && // 50: 
        //        (global_UI_MouseState.LClick_y >= 0) &&
        //        (global_UI_MouseState.LClick_y <= global_UI_MouseState.TABBAR_HEIGHT)
        //    )
        if (UIFunc_isMouseOver_TopBar() && global_UI_Topbar.pressed)
           {
                // TODO: COLOR CHANGE ON TABBAR CLICK
                // global_UI_Topbar.pressed = true;
                global_UI_Topbar.color = COLOR_TABBAR_CLICKED;
                // global_UI_MouseState.TABBAR_CLICK = true;
                // global_UI_MouseState.TABBAR_COLOR = COLOR_TABBAR_CLICKED;
                // color = global_UI_MouseState.TABBAR_COLOR;
                tabbarClick(handleWindow);
           } 
    }  else {
        // global_UI_MouseState.TABBAR_CLICK = false;
        // global_UI_MouseState.TABBAR_COLOR = COLOR_TABBAR_DEFAULT;
        global_UI_Topbar.color = COLOR_TABBAR_DEFAULT;
    }

    for(i32 y = 0; y < global_UI_Topbar.height; y++){
        for (i32 x = 0; x < global_UI_Topbar.width; x++){
            pixel[x] = global_UI_Topbar.color;
            // pixel[x] = color;
        }
        pixel += width;
    }

    // Close Button:
    global_UI_MouseState.CLOSEBTN_WIDTH = 50;
    global_UI_MouseState.CLOSEBTN_HEIGHT = global_UI_Topbar.height;
    global_UI_MouseState.CLOSEBTN_POS_X = (width - global_UI_MouseState.CLOSEBTN_WIDTH);
    global_UI_MouseState.CLOSEBTN_POS_Y = 0;
    UI_ButtonClose(
        global_UI_MouseState.CLOSEBTN_POS_X, 
        global_UI_MouseState.CLOSEBTN_POS_Y, 
        global_UI_MouseState.CLOSEBTN_WIDTH, 
        global_UI_MouseState.CLOSEBTN_HEIGHT 
    );
}

// Components:
void UI_Button(
    i32 x, i32 y, i32 width, i32 height,
    i32 (*colorOnClick)()
){
    i32 color = 0x00aaffff;
    // Detecting Click
    if (global_UI_MouseState.LClick) {
        if (
            (global_UI_MouseState.LClick_x >= x) && 
            (global_UI_MouseState.LClick_x <= x+width) &&
            (global_UI_MouseState.LClick_y >= y) &&
            (global_UI_MouseState.LClick_y <= y+height)
        ){
            if (!colorOnClick)
                color = 0x00f75d1e;
            else
                color = colorOnClick();
        }
    }

    u32* pixel =  (
        (u32*)global_UI_BackBuffer.memory + y * global_UI_BackBuffer.width + x
    );
    
    for(i32 y = 0; y < height; y++) {
        for(i32 i = 0; i < width; i++){
            if (
                // (y < gameBackBuffer->height - globalBox.height) 
                (y < global_UI_BackBuffer.height - height) 
                || (y > 0))
                *(pixel+i) = color;
        }
        // if ((y < globalBackBuffer.height - globalBox.height) || (y > 0))
        if ((y < global_UI_BackBuffer.height - height) || (y > 0))
            pixel += global_UI_BackBuffer.width;
    }
}



void UI_ButtonClose(
    i32 x, i32 y, i32 width, i32 height
){
    i32 color = 0x00ff0000;
    i32 click_color = 0x00aa0000;
    
    // Detecting Click:
    if (global_UI_MouseState.LClick) {
        // global_UI_MouseState.CLOSEBTN_CLICK = true;
        if (
            (global_UI_MouseState.LClick_x >= x) && 
            (global_UI_MouseState.LClick_x <= x+width) &&
            (global_UI_MouseState.LClick_y >= y) &&
            (global_UI_MouseState.LClick_y <= y+height)
        ){
            color = click_color;
            global_UI_MouseState.CLOSEBTN_CLICK = true;
        }
    }

    u32* pixel =  (
        (u32*)global_UI_BackBuffer.memory + y * global_UI_BackBuffer.width + x
    );
    
    for(i32 y = 0; y < height; y++) {
        for(i32 i = 0; i < width; i++){
            if (
                (y < global_UI_BackBuffer.height - height) 
                || (y > 0))
                *(pixel+i) = color;
        }
        if ((y < global_UI_BackBuffer.height - height) || (y > 0))
            pixel += global_UI_BackBuffer.width;
    }

    // Draw X:
    i32 close_btn_width = width ;
    i32 close_btn_height = height;
    PLATFORM_IMPL_DrawText(
        "X", 
        x + (width/2 - close_btn_width/2),
        y + (height/2 - close_btn_height/2),
        close_btn_width, close_btn_height
    );
}



// Funcs:

bool UIFunc_isCursorOnCloseBtn(){
    if (
        (global_UI_MouseState.LClick_x >= global_UI_MouseState.CLOSEBTN_POS_X) 
        && 
        (global_UI_MouseState.LClick_x <= global_UI_MouseState.CLOSEBTN_POS_X + global_UI_MouseState.CLOSEBTN_WIDTH) 
        &&
        (global_UI_MouseState.LClick_y >= global_UI_MouseState.CLOSEBTN_POS_Y) 
        &&
        (global_UI_MouseState.LClick_y <= global_UI_MouseState.CLOSEBTN_POS_Y + global_UI_MouseState.CLOSEBTN_HEIGHT)
    ){
        return true;
    }
    return false;
}


// bool UIFunc_isTabBarClick() {
bool UIFunc_isMouseOver_TopBar() {
    // if (
    //     (global_UI_MouseState.LClick_x >= global_UI_MouseState.TABBAR_X) 
    //     && 
    //     (global_UI_MouseState.LClick_x <= global_UI_MouseState.TABBAR_X + global_UI_MouseState.TABBAR_WIDTH) 
    //     &&
    //     (global_UI_MouseState.LClick_y >= global_UI_MouseState.TABBAR_Y) 
    //     &&
    //     (global_UI_MouseState.LClick_y <= global_UI_MouseState.TABBAR_Y + global_UI_MouseState.TABBAR_HEIGHT)
    //     && 
    //     (global_UI_MouseState.TABBAR_CLICK)
    // ){
    //     return true;
    // }

    if (
        (global_UI_MouseState.LClick_x >= 0) &&
        (global_UI_MouseState.LClick_x <= global_UI_BackBuffer.width)
        && 
        (global_UI_MouseState.LClick_y >= 0) && 
        (global_UI_MouseState.LClick_y <= global_UI_Topbar.height)
    ){
        return true;
    }


    return false;
}

#endif