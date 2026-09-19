#include "skb_types.h"

struct UI_BackBuffer{
    int width;
    int height;
    void* memory; 
    const int bytesPerPixel = 4; 
};

struct UI_MouseState {
    bool LClick = false;
    bool RClick = false;
    int LClick_x = -1; 
    int LClick_y = -1; 
};

UI_BackBuffer global_UIBackBuffer;
UI_MouseState global_UIMouseState;

void UI_FillBackground(UI_BackBuffer *backbuffer, i32 color);
void UI_Button(int x, int y, int width, int height, i32 (*func)());
void UI_TabBar(int height);

void PLATFORM_IMPL_DrawText(const char* text, int x, int y, int width, int height);

/***************************************************** */
// Implementation:=>
/***************************************************** */
#ifdef __UI__

void UI_FillBackground(UI_BackBuffer *backbuffer, i32 color){
    for(int i = 0; i < backbuffer->width * backbuffer->height; i++) {
        ((i32*)backbuffer->memory)[i] = color;
    }
}

void UI_ButtonClose(int x, int y, int width = 40, int height = 40){
    u32* pixel = (u32*)global_UIBackBuffer.memory + global_UIBackBuffer.width*y + x;
    u32 color = 0x00ff0000;

    if (global_UIMouseState.LClick) {
        if (
            (global_UIMouseState.LClick_x >= x) && 
            (global_UIMouseState.LClick_x <= x+width) &&
            (global_UIMouseState.LClick_y >= y) &&
            (global_UIMouseState.LClick_y <= y+height)
        ){
            color = 0x00880000;
        }
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            pixel[j] = color;
        }
        pixel += (global_UIBackBuffer.width);
    }

    // Draw X:
    int close_btn_width = width ;//- 5;
    int close_btn_height = height;// - 5;
    PLATFORM_IMPL_DrawText(
        "X", 
        x + (width/2 - close_btn_width/2),
        y + (height/2 - close_btn_height/2),
        close_btn_width, close_btn_height
    );
}

void UI_TabBar(int height = 50){
    int width = global_UIBackBuffer.width;
    u32* pixel = (u32*)global_UIBackBuffer.memory;
    u32 color = 0x00323233;

    for(int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            pixel[x] = color;
        }
        pixel += width;
    }

    // Close Button:
    int closebtn_width = 50;
    int closebtn_height = 50;
    UI_ButtonClose(width - closebtn_width, 0, closebtn_width, closebtn_width);
}

// Components:
void UI_Button(
    int x, int y, int width, int height,
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
    
    for(int y = 0; y < height; y++) {
        for(int i = 0; i < width; i++){
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

#endif