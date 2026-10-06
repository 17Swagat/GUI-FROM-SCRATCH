#pragma once

#include "skb_types.h"
// #include "assert.h"

// #define COLOR_BG_COLOR 0x00440022
#define COLOR_BG_COLOR 0xff440022
#define COLOR_TABBAR_DEFAULT 0xff323233
#define COLOR_TABBAR_CLICKED 0xff543233
#define COLOR_BUTTON_DEFAULT 0xff555555
// #define COLOR_BUTTON_CLICKED 0xffaa00a0
#define COLOR_CLOSEBTN_DEFAULT 0xffff0000
#define COLOR_CLOSEBTN_CLICKED 0xffaa0000 

// Win32
struct PLATFORM_Type_TEXTDIM // TextDimensions
{
    long width;
    long height;
};

struct UI_BackBuffer {
    i32 width;
    i32 height;
    void* memory;
    const i32 bytesPerPixel = 4;
};

struct UI_Topbar {
    i32 x;
    i32 y;
    u32 width;
    u32 height;
    u32 color;
    bool pressed = false;
    bool draggingWindow = false;
    struct { long x; long y; } mousePoint;
    struct { long x; long y; } windowStart;
};

struct UI_MouseState {
    bool LClick = false;
    bool RClick = false;
    i32 LClick_x = -1;
    i32 LClick_y = -1;

    // TabBar
    bool TABBAR_CLICK = false;

    // CLOSE [X] Btn:
    bool CLOSEBTN_CLICK = false;
    // Will Get Filled:
    i32 CLOSEBTN_POS_X = 0;
    i32 CLOSEBTN_POS_Y = 0;
    u32 CLOSEBTN_WIDTH = 0;
    u32 CLOSEBTN_HEIGHT = 0;
};

// TODO: Need better management of Keyboard i/p: Need to be deal with multiple key presses at a time and need to reflect in button presses.
struct UI_KeyboardState {
    bool pressed = false;
    char key = '\0';
};

// ME: Better to have way to deal with all the keys through strings (const char*)
// enum t_Key { // t_ : type
//     KEY_XX = '\0' // No Key pressed.
//     // ,KEY_0 = '0', KEY_1 = '1', KEY_2 = '2', KEY_3 = '3', KEY_4 = '4', KEY_5 = '5', KEY_6 = '6', KEY_7 = '7', KEY_8 = '8', KEY_9 = '9'
// };

// enum t_KeyTextInfo {
//     tKEY_CHARA, // char array
//     tKEY_KEY // Key Type
// };

// struct KeyText {
//     union {
//         t_Key key = KEY_XX;
//         char txt[200];
//         // char txt[16];
//     };

//     t_KeyTextInfo type = tKEY_KEY;

//     bool operator == (const KeyText& other) const
//     {
//         return (type == other.type && key == other.key);
//     }
// };

// struct UI_KeyboardState_2 {
//     bool pressed = false;
//     KeyText keytext;
// };

struct Button {
    i32 x; i32 y; i32 width = 50; i32 height = 50;
    // **Important for Keyboard Input**
    u32 hotKey; 
    const char* text;
    // Defaults:
    double scale = 1.0;
    // u32 clickColor = COLOR_BUTTON_CLICKED;
    bool pressed = false;
    // When The button get's pressed we would want to reduce the size of the fonts. Instead of re-creating fonts of the specified size again and again, the win32 platform layer currently stores different fonts (right now same type of fonts, but with differnet sizes). When button gets clicked the smaller font gets used.
    i32 fontIndex = 0; 
};

// Globals:
UI_BackBuffer global_UI_BackBuffer;
UI_MouseState global_UI_MouseState;
UI_KeyboardState global_UI_KeyboardState;
UI_Topbar global_UI_Topbar;
// UI_KeyboardState_2 global_UI_KeyboardState_2;

// Platform:
// PLATFORM_Type_TEXTDIM PLATFORM_GET_TEXTDIMS(const char* text); // TODO: DELETE THIS 
PLATFORM_Type_TEXTDIM PLATFORM_GET_TEXTDIMS(const char* text, i32 fontIndex);
void PLATFORM_IMPL_RenderTextToButton(i32 btn_x, i32 btn_y, i32 btn_w, i32 btn_h,
                                      i32 txt_width, i32 txt_height, const char* txt,
                                      i32 fontIndex);

// UI:
void UI_Button(Button* button, const char* text);
void UI_FillBackground(UI_BackBuffer* backbuffer, i32 color);
void UI_ButtonClose(i32 x, i32 y, i32 width, i32 height);
void UI_TopBar();

// UI Funcs:
bool UIFunc_mouseClick(int x, int y, int w, int h);
bool UIFunc_isMouseOver_TopBar();
bool UIFunc_isCursorOnCloseBtn();

/***************************************************** */
// Implementation:=>
/***************************************************** */
// TODO: Implement
/* 
NOTE:
Google [Win32 Virtual-Key (VK_) Code Ranges ]:
LINK: https://share.google/aimode/O8tZWjpkASdFtbhEj
In the Win32 API, virtual-key codes (**VK_ codes**) are defined as 8-bit values. This means the absolute theoretical range spans from **0x00 to 0xFF** (0 to 255 in decimal). 
*Note: `0x00` and `0xFF` are officially unassigned/defined as "Undefined" or reserved by the system.*

## Virtual-Key Code Range Breakdown
| Hex Range | Decimal Range | General Purpose / Key Types |
| :---            | :---    | :---                        |
| **0x01 – 0x07** | 1 – 7   | Mouse buttons and special system actions (e.g., Cancel) |
| **0x08 – 0x2F** | 8 – 47  | Control keys (Backspace, Tab, Enter, Shift, Ctrl, Alt, Pause, Caps Lock, Escape, Space, Page Up/Down, End, Home, Arrow keys) |

| **0x30 – 0x39** | 48 – 57 | Standard **0 through 9** keys (matches ASCII `'0'` through `'9'`) |
| **0x41 – 0x5A** | 65 – 90 | Standard **A through Z** keys (matches ASCII `'A'` through `'Z'`) |

| **0x5B – 0x6F** | 91 – 111 | Windows keys, Applications key, and **Numpad** keys (0-9, Multiply, Add, Separator, Subtract, Decimal, Divide) |
| **0x70 – 0x87** | 112 – 135 | Function keys (**F1 through F24**) |
| **0x90 – 0x96** | 144 – 150 | Lock keys (Num Lock, Scroll Lock) and specific Apple/NEC standard mappings |
| **0xA0 – 0xB9** | 160 – 185 | Left/Right modifier keys (LSHIFT, RSHIFT, LCONTROL, RCONTROL, etc.) and Browser/Media control keys |
| **0xBA – 0xC0** | 186 – 192 | Miscellaneous OEM punctuation keys (`;`, `=`, `,`, `-`, `.`, `/`, `` ` ``) *depending on keyboard layout* |
| **0xDB – 0xDF** | 219 – 223 | Additional OEM bracket/slash keys (`[`, `\`, `]`, `'`) |
| **0xE1 – 0xFE** | 225 – 254 | OEM-specific or extended keys (e.g., IME keys, `VK_PACKET` for keystroke passing, `VK_OEM_CLEAR`) |
*/

struct KeyState {
    // Win32 Keys
    bool press[255] = {0}; // 0x00 To 0xFF, [Where 0x00 & 0xFF are reserved in Win32]
    bool down[255] = {0};
};

KeyState global_Input_KeyboardState;

void PLATFORM_INPUT_KEYBOARD(); // TEMPORARY: PLACE

#ifdef __UI__

// TODO: 
// 1. Simply button click through mouse
// 2. Clean Keyboard click
// 3. Enable Multiple Clicks (Architecture needs to change for this)

void UI_Button(Button* button, const char* text)
{
    int x = button->x;
    int y = button->y;
    int width = button->width;
    int height = button->height;
    u32 color = COLOR_BUTTON_DEFAULT;

    PLATFORM_Type_TEXTDIM textdims;
    textdims = PLATFORM_GET_TEXTDIMS(text, button->fontIndex);
    if ((textdims.width) > (width * 0.80)) {
        // debugPrint("%s %d: \n", __FILE__, __LINE__);
        // debugPrint("ISSUE: Adviced to Resize the button according to your text\n");
        while ((textdims.width) > (width * 0.80)) {
            width += width * 0.25;
        }
        button->width = width;
        // RED color to show this button's (width,height) needs to be reconsiderd:
        color = 0xffff0000; 
    }
    

    // @LATER: (Improvise) But it works
    // Keyboard Click: 
    bool btn_press_keyboard = false;
    if(global_Input_KeyboardState.press[button->hotKey]){
        btn_press_keyboard = true;
    }
    // if (global_UI_KeyboardState.pressed) {
    //     if (global_UI_KeyboardState_2.keytext == button->triggerKey) {
    //         triggerButton = true;
    //     }
    // }

    // Detecting Mouse Click:
    if (UIFunc_mouseClick(x, y, width, height)) {
        button->pressed = true;
        button->fontIndex = 1;
    } 

    if (button->pressed || btn_press_keyboard) {
        width = width * 0.95;
        height = height * 0.95;
        x = x + button->width/2 - width/2;
        y = y + button->height/2 - height/2;
        // Text Dimension changing:
        textdims = PLATFORM_GET_TEXTDIMS(text, button->fontIndex);
    }


    // Draw:
    u32* pixel = (
        (u32*)global_UI_BackBuffer.memory + y * global_UI_BackBuffer.width + x
        );

    for (i32 y = 0; y < height; y++) {
        for (i32 i = 0; i < width; i++) {
            if (
                (y <= global_UI_BackBuffer.height - height)
                ||
                (y >= 0)
                )
                *(pixel + i) = color; //button->clickColor;
        }
        if ((y < global_UI_BackBuffer.height - height) || (y > 0))
            pixel += global_UI_BackBuffer.width;
    }

    // TODO: Making Fonts smaller on Clicks. Right now having a const Font Size is Ok. Will have to do changes in the Win32 Platform Layer
    // const char* temp_text = "Hello";
    PLATFORM_IMPL_RenderTextToButton(
        x, y, width, height,
        textdims.width, textdims.height, text,
        button->fontIndex
    );
}

void UI_FillBackground(UI_BackBuffer* backbuffer, i32 color) {
    for (i32 i = 0; i < backbuffer->width * backbuffer->height; i++) {
        ((i32*)backbuffer->memory)[i] = color;
    }
}

void UI_TopBar() {
    i32 width = global_UI_BackBuffer.width;
    u32* pixel = (u32*)global_UI_BackBuffer.memory;

    if (global_UI_MouseState.LClick) {
        if (UIFunc_isMouseOver_TopBar() && global_UI_Topbar.pressed)
        {
            global_UI_Topbar.color = COLOR_TABBAR_CLICKED;
        }
    }
    else {
        global_UI_Topbar.color = COLOR_TABBAR_DEFAULT;
    }

    for (i32 y = 0; y < global_UI_Topbar.height; y++) {
        for (i32 x = 0; x < global_UI_Topbar.width; x++) {
            pixel[x] = global_UI_Topbar.color;
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


void UI_ButtonClose(i32 x, i32 y, i32 width, i32 height) {
    u32 color = COLOR_CLOSEBTN_DEFAULT;
    static_local PLATFORM_Type_TEXTDIM textdims = PLATFORM_GET_TEXTDIMS("X", 0);

    // Detecting Click:
    if (global_UI_MouseState.LClick) {
        // global_UI_MouseState.CLOSEBTN_CLICK = true;
        if (
            (global_UI_MouseState.LClick_x >= x) &&
            (global_UI_MouseState.LClick_x <= x + width) &&
            (global_UI_MouseState.LClick_y >= y) &&
            (global_UI_MouseState.LClick_y <= y + height)
        ) {
            global_UI_MouseState.CLOSEBTN_CLICK = true;
            color = COLOR_CLOSEBTN_CLICKED;
        }
    }

    u32* pixel = (
        (u32*)global_UI_BackBuffer.memory + y * global_UI_BackBuffer.width + x
        );

    for (i32 y = 0; y < height; y++) {
        for (i32 i = 0; i < width; i++) {
            if (
                (y < global_UI_BackBuffer.height - height)
                || (y > 0))
                *(pixel + i) = color;
        }
        if ((y < global_UI_BackBuffer.height - height) || (y > 0))
            pixel += global_UI_BackBuffer.width;
    }

    // Draw X:
    PLATFORM_IMPL_RenderTextToButton(
        x, y, width, height,
        textdims.width, textdims.height,
        "X", 1 // 0: Default Font
    );
}

// UI Funcs:
bool UIFunc_mouseClick(int x, int y, int w, int h) {
    if (global_UI_MouseState.LClick &&
        (global_UI_MouseState.LClick_x >= x) &&
        (global_UI_MouseState.LClick_x <= x + w) &&
        (global_UI_MouseState.LClick_y >= y) &&
        (global_UI_MouseState.LClick_y <= y + h)
        ) {
        // button->pressed = true;
        return true;
    }
    return false;
}

bool UIFunc_isCursorOnCloseBtn() {
    if (
        (global_UI_MouseState.LClick_x >= global_UI_MouseState.CLOSEBTN_POS_X)
        &&
        (global_UI_MouseState.LClick_x <= global_UI_MouseState.CLOSEBTN_POS_X + global_UI_MouseState.CLOSEBTN_WIDTH)
        &&
        (global_UI_MouseState.LClick_y >= global_UI_MouseState.CLOSEBTN_POS_Y)
        &&
        (global_UI_MouseState.LClick_y <= global_UI_MouseState.CLOSEBTN_POS_Y + global_UI_MouseState.CLOSEBTN_HEIGHT)
        ) {
        return true;
    }
    return false;
}

bool UIFunc_isMouseOver_TopBar() {
    if (
        (global_UI_MouseState.LClick_x >= 0) &&
        (global_UI_MouseState.LClick_x <= global_UI_BackBuffer.width - global_UI_MouseState.CLOSEBTN_WIDTH)
        &&
        (global_UI_MouseState.LClick_y >= 0) &&
        (global_UI_MouseState.LClick_y <= global_UI_Topbar.height)
        ) {
        return true;
    }


    return false;
}

#endif