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
    UI_TopBar();
        
    static_local const i32 clientArea_x_start = 0 + 20; // start from 20
    static_local const i32 clientArea_y_start = global_UI_Topbar.height + 10;
    static const i32 spaceBetween1 = 5;



}

#endif