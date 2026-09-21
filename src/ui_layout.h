#pragma once

#include "skb_types.h"
#define __UI__
#include "ui.h"

#ifdef __UILAYOUT__

void UI_LAYOUT(
    void* handleWindow,
    void (*tabbarClick)(void*)
){

    i32 bgColor = 0x00aaaaaa;
    UI_FillBackground(&global_UIBackBuffer, bgColor);
    UI_TabBar(
        [](void*data){global_UIMouseState.CLOSEBTN_CLICK = true;}, // win32_closeApp,
        // For Tab Click:
        handleWindow,
        tabbarClick
    );
        
    UI_Button(50, 300, 200, 40, NULL);
    // UI_Button(100, 100, 100, 100, func_colorChange);

    UI_Button(global_UIBackBuffer.width/2, global_UIBackBuffer.height/2, 150, 150, 
        [](){return 0x00ffff00;} 
    );
}

#endif