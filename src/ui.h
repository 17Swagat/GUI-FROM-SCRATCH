#pragma once

#include "skb_types.h"

struct UI_BackBuffer{
    i32 width;
    i32 height;
    void* memory; 
    const i32 bytesPerPixel = 4; 
};

struct UI_MouseState {
    bool LClick = false;
    bool RClick = false;
    i32 LClick_x = -1; 
    i32 LClick_y = -1; 

    // CLOSE [X] Btn:
    bool CLOSEBTN_CLICK = false;
    i32 CLOSEBTN_POS_X = 0;
    i32 CLOSEBTN_POS_Y = 0;
    i32 CLOSEBTN_WIDTH = 0;
    i32 CLOSEBTN_HEIGHT = 0;
};

UI_BackBuffer global_UIBackBuffer;
UI_MouseState global_UIMouseState;

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
    void (*tabbarClick)(void*),
    i32 height
);

bool UIFunc_isCursorOnCloseBtn();

// Will Get Implemented by the Platform Layer:
void PLATFORM_IMPL_DrawText(const char* text, i32 x, i32 y, i32 width, i32 height);


/***************************************************** */
// Implementation:=>
/***************************************************** */
#ifdef __UI__

void UI_FillBackground(UI_BackBuffer *backbuffer, i32 color){
    for(i32 i = 0; i < backbuffer->width * backbuffer->height; i++) {
        ((i32*)backbuffer->memory)[i] = color;
    }
}

void UI_TabBar(
    void (*closeApp)(void*), 
    void* handleWindow,
    void (*tabbarClick)(void*),
    i32 height = 50
) {
    i32 width = global_UIBackBuffer.width;
    u32* pixel = (u32*)global_UIBackBuffer.memory;
    u32 color = 0x00323233;

    for(i32 y = 0; y < height; y++){
        for (i32 x = 0; x < width; x++){
            pixel[x] = color;
        }
        pixel += width;
    }

    if (global_UIMouseState.LClick) {
           if (
               (global_UIMouseState.LClick_x >= 0) && 
               (global_UIMouseState.LClick_x <= global_UIBackBuffer.width - 50) && // 50: 
               (global_UIMouseState.LClick_y >= 0) &&
               (global_UIMouseState.LClick_y <= height)
           ){
                // color = 0x00f75d1e;
                // Move App Around:
                tabbarClick(handleWindow);
           }
    }

    // Close Button:
    global_UIMouseState.CLOSEBTN_WIDTH = 50;
    global_UIMouseState.CLOSEBTN_HEIGHT = height;
    global_UIMouseState.CLOSEBTN_POS_X = (width - global_UIMouseState.CLOSEBTN_WIDTH);
    global_UIMouseState.CLOSEBTN_POS_Y = 0;
    UI_ButtonClose(
        global_UIMouseState.CLOSEBTN_POS_X, 
        global_UIMouseState.CLOSEBTN_POS_Y, 
        global_UIMouseState.CLOSEBTN_WIDTH, 
        global_UIMouseState.CLOSEBTN_HEIGHT 
    );
}

// Components:
void UI_Button(
    i32 x, i32 y, i32 width, i32 height,
    i32 (*colorOnClick)()
){
    i32 color = 0x00aaffff;
    // Detecting Click
    if (global_UIMouseState.LClick) {
        if (
            (global_UIMouseState.LClick_x >= x) && 
            (global_UIMouseState.LClick_x <= x+width) &&
            (global_UIMouseState.LClick_y >= y) &&
            (global_UIMouseState.LClick_y <= y+height)
        ){
            if (!colorOnClick)
                color = 0x00f75d1e;
            else
                color = colorOnClick();
        }
    }

    u32* pixel =  (
        (u32*)global_UIBackBuffer.memory + y * global_UIBackBuffer.width + x
    );
    
    for(i32 y = 0; y < height; y++) {
        for(i32 i = 0; i < width; i++){
            if (
                // (y < gameBackBuffer->height - globalBox.height) 
                (y < global_UIBackBuffer.height - height) 
                || (y > 0))
                *(pixel+i) = color;
        }
        // if ((y < globalBackBuffer.height - globalBox.height) || (y > 0))
        if ((y < global_UIBackBuffer.height - height) || (y > 0))
            pixel += global_UIBackBuffer.width;
    }
}


bool UIFunc_isCursorOnCloseBtn(){
    if (
        (global_UIMouseState.LClick_x >= global_UIMouseState.CLOSEBTN_POS_X) 
        && 
        (global_UIMouseState.LClick_x <= global_UIMouseState.CLOSEBTN_POS_X + global_UIMouseState.CLOSEBTN_WIDTH) 
        &&
        (global_UIMouseState.LClick_y >= global_UIMouseState.CLOSEBTN_POS_Y) 
        &&
        (global_UIMouseState.LClick_y <= global_UIMouseState.CLOSEBTN_POS_Y + global_UIMouseState.CLOSEBTN_HEIGHT)
    ){
        return true;
    }
    return false;
}


void UI_ButtonClose(
    i32 x, i32 y, i32 width, i32 height
){
    i32 color = 0x00ff0000;
    i32 click_color = 0x00aa0000;
    
    // Detecting Click:
    if (global_UIMouseState.LClick) {
        // global_UIMouseState.CLOSEBTN_CLICK = true;
        if (
            (global_UIMouseState.LClick_x >= x) && 
            (global_UIMouseState.LClick_x <= x+width) &&
            (global_UIMouseState.LClick_y >= y) &&
            (global_UIMouseState.LClick_y <= y+height)
        ){
            color = click_color;
            global_UIMouseState.CLOSEBTN_CLICK = true;
        }
    }

    u32* pixel =  (
        (u32*)global_UIBackBuffer.memory + y * global_UIBackBuffer.width + x
    );
    
    for(i32 y = 0; y < height; y++) {
        for(i32 i = 0; i < width; i++){
            if (
                (y < global_UIBackBuffer.height - height) 
                || (y > 0))
                *(pixel+i) = color;
        }
        if ((y < global_UIBackBuffer.height - height) || (y > 0))
            pixel += global_UIBackBuffer.width;
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


#endif