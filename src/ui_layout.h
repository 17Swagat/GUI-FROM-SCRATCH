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
        
    // UI_Button(50, 300, 200, 40, NULL);
    // UI_Button(100, 100, 100, 100, func_colorChange);

    // UI_Button(global_UI_BackBuffer.width/2, global_UI_BackBuffer.height/2, 150, 150, 
    //     [](){return 0x00ffff00;} 
    // );

    /*
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
    */

    static_local const i32 clientArea_x_start = 0;
    static_local const i32 clientArea_y_start = global_UI_Topbar.height + 10;
    static const i32 spaceBetween1 = 5;

    // DRAWING BUTTONS (Layout):
    // Number Keys:
    ButtonPressed btn_1 = {
        .x = 20, .y = clientArea_y_start, 
        .width=50, .height=50, 
        .triggerKey='1'
    };
    UI_ButtonPressed(&btn_1, deltaTime);

    ButtonPressed btn_2 = {
        .x = btn_1.x + btn_1.width + spaceBetween1, 
        .y = clientArea_y_start, 
        .width=50, .height=50, 
        .triggerKey='2'
    };
    UI_ButtonPressed(&btn_2, deltaTime);

    ButtonPressed btn_3 = {
        .x = btn_2.x + btn_2.width + spaceBetween1, 
        .y = clientArea_y_start, 
        .width=50, .height=50, 
        .triggerKey='3'
    };
    UI_ButtonPressed(&btn_3, deltaTime);

    ButtonPressed btn_4 = {
        .x = btn_3.x + btn_3.width + spaceBetween1, 
        .y = clientArea_y_start, 
        .width=50, .height=50, 
        .triggerKey='4'
    };
    UI_ButtonPressed(&btn_4, deltaTime);

    ButtonPressed btn_5 = {
        .x = btn_4.x + btn_4.width + spaceBetween1, 
        .y = clientArea_y_start, 
        .width=50, .height=50, 
        .triggerKey='5'
    };
    UI_ButtonPressed(&btn_5, deltaTime);

    ButtonPressed btn_6 = {
        .x = btn_5.x + btn_5.width + spaceBetween1, 
        .y = clientArea_y_start, 
        .width=50, .height=50, 
        .triggerKey='6'
    };
    UI_ButtonPressed(&btn_6, deltaTime);


    ButtonPressed btn_7 = {
        .x = btn_6.x + btn_6.width + spaceBetween1, 
        .y = clientArea_y_start, 
        .width=50, .height=50, 
        .triggerKey='7'
    };
    UI_ButtonPressed(&btn_7, deltaTime);

    // ButtonPressed btn_7 = {
    //     .x = btn_6.x + btn_6.width + spaceBetween1, 
    //     .y = clientArea_y_start, 
    //     .width=50, .height=50, 
    //     .triggerKey='7'
    // };
    // UI_ButtonPressed(&btn_7, deltaTime);

    // ButtonPressed btn_7 = {
    //     .x = btn_6.x + btn_6.width + spaceBetween1, 
    //     .y = clientArea_y_start, 
    //     .width=50, .height=50, 
    //     .triggerKey='7'
    // };
    // UI_ButtonPressed(&btn_7, deltaTime);

    // ButtonPressed btn_4 = {.x = btn_3.x + btn_3.width + spaceBetween1, .y = clientArea_y_start, .width=50, .height=50, .triggerKey='0'};
    // UI_ButtonPressed(&btn_4, deltaTime);
    // ButtonPressed btn_5 = {.x = btn_4.x + btn_4.width + spaceBetween1, .y = clientArea_y_start, .width=50, .height=50, .triggerKey='5'};
    // UI_ButtonPressed(&btn_5, deltaTime);
    // ButtonPressed btn_6 = {.x = btn_5.x + btn_5.width + spaceBetween1, .y = clientArea_y_start, .width=50, .height=50, .triggerKey='6'};
    // UI_ButtonPressed(&btn_6, deltaTime);
    // ButtonPressed btn_7 = {.x = btn_6.x + btn_6.width + spaceBetween1, .y = clientArea_y_start, .width=50, .height=50, .triggerKey='7'};
    // UI_ButtonPressed(&btn_7, deltaTime);
    // ButtonPressed btn_8 = {.x = btn_7.x + btn_7.width + spaceBetween1, .y = clientArea_y_start, .width=50, .height=50, .triggerKey='8'};
    // UI_ButtonPressed(&btn_8, deltaTime);
    // ButtonPressed btn_9 = {.x = btn_8.x + btn_8.width + spaceBetween1, .y = clientArea_y_start, .width=50, .height=50, .triggerKey='9'};
    // UI_ButtonPressed(&btn_9, deltaTime);
    // ButtonPressed btn_0 = {.x = btn_9.x + btn_9.width + spaceBetween1, .y = clientArea_y_start, .width=50, .height=50, .triggerKey='0'};
    // UI_ButtonPressed(&btn_0, deltaTime);
}

#endif