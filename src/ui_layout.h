#pragma once

#include "skb_types.h"
#include "ui.h"


void UI_LAYOUT(
    void* handleWindow,
    double deltaTime,
    void (*tabbarClick)(void*)
);

#ifdef __UILAYOUT__

i32 func_colorChange(){
    i32 color = 0x00f7fdae;
    return color;
}

static_func void init(){
    global_UIMouseState.TABBAR_X = 0;
    global_UIMouseState.TABBAR_Y = 0;
    global_UIMouseState.TABBAR_WIDTH = global_UIBackBuffer.width;
    global_UIMouseState.TABBAR_HEIGHT = 50;
}

void UI_LAYOUT(
    void* handleWindow,
    double deltaTime,
    void (*tabbarClick)(void*)
){

    i32 bgColor = 0x00000000;
    init();

    UI_FillBackground(&global_UIBackBuffer, bgColor);
    UI_TabBar(
        [](void*data){global_UIMouseState.CLOSEBTN_CLICK = true;}, // win32_closeApp,
        // For Tab Click:
        handleWindow,
        tabbarClick,
        global_UIMouseState.TABBAR_HEIGHT
    );
        
    // UI_Button(50, 300, 200, 40, NULL);
    // UI_Button(100, 100, 100, 100, func_colorChange);

    // UI_Button(global_UIBackBuffer.width/2, global_UIBackBuffer.height/2, 150, 150, 
    //     [](){return 0x00ffff00;} 
    // );

    // BUTTON PRESS ANIMATION
    // static bool pressed = false;
    int x = 100;
    int y = 100;
    int w = 150;
    int h = 150;
    ButtonPressed btn = {
        .x = x, .y = y, .width = w, .height = h,
        .triggerKey = 'A'
    };
    UI_ButtonPressed(&btn, deltaTime);
}

#endif