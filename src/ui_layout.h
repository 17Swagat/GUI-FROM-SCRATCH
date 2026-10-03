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

static_global bool initStuff = false;
static_func void init(){
    if (!initStuff) {
        // global_UI_MouseState.TABBAR_X = 0;
        // global_UI_MouseState.TABBAR_Y = 0;
        // global_UI_MouseState.TABBAR_WIDTH = global_UI_BackBuffer.width;
        // global_UI_MouseState.TABBAR_HEIGHT = 50;
        global_UI_Topbar.x = 0;
        global_UI_Topbar.y = 0;
        global_UI_Topbar.width = global_UI_BackBuffer.width;
        global_UI_Topbar.height = 50;
        // global_UI_Topbar.pressed = false;
        // global_UI_Topbar.dragged = false;
        initStuff = true;
    }
}

void UI_LAYOUT(
    void* handleWindow,
    double deltaTime,
    void (*tabbarClick)(void*)
){
    if (!initStuff) {
        init();
    }

    UI_FillBackground(&global_UI_BackBuffer, COLOR_BG_COLOR);
    UI_TabBar(
        // win32_closeApp:
        [](void*data){global_UI_MouseState.CLOSEBTN_CLICK = true;}, 
        // For Tab Click:
        handleWindow,
        tabbarClick
        // ,global_UI_MouseState.TABBAR_HEIGHT
    );
        
    // UI_Button(50, 300, 200, 40, NULL);
    // UI_Button(100, 100, 100, 100, func_colorChange);

    // UI_Button(global_UI_BackBuffer.width/2, global_UI_BackBuffer.height/2, 150, 150, 
    //     [](){return 0x00ffff00;} 
    // );

    // BUTTON PRESS ANIMATION
    // #1
    int x = 100;
    int y = 100;
    int w = 150;
    int h = 150;
    ButtonPressed btn = {
        .x = x, .y = y, .width = w, .height = h,
        .triggerKey = 'A'
    };
    UI_ButtonPressed(&btn, deltaTime);

    // #2
    ButtonPressed btn2 = {.x = 350, .y=200, .width=300, .height= 220, .triggerKey='W'};
    UI_ButtonPressed(&btn2, deltaTime);
}

#endif