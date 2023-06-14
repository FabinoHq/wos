////////////////////////////////////////////////////////////////////////////////
//   _______                               ________________________________   //
//   \\ .   \                     ________/ . . . . . . . . . . . . . .   /   //
//    \\ .   \     ____       ___/ . . . . .   __________________________/    //
//     \\ .   \   //   \   __/. . .  _________/   /    // .  _________/       //
//      \\ .   \_//     \_//     ___/.  _____    /    // .  /_____            //
//       \\ .   \/   _   \/    _/// .  /    \\   |    \\  .       \           //
//        \\ .      /\\       /  || .  |    ||   |     \\______    \          //
//         \\ .    /  \\     /   || .  \____//   |  _________//    /          //
//          \\ .  /    \\   /    //  .           / // . . . .     /           //
//           \\__/      \\_/    //______________/ //_____________/            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//   This is free and unencumbered software released into the public domain.  //
//                                                                            //
//   Anyone is free to copy, modify, publish, use, compile, sell, or          //
//   distribute this software, either in source code form or as a compiled    //
//   binary, for any purpose, commercial or non-commercial, and by any        //
//   means.                                                                   //
//                                                                            //
//   In jurisdictions that recognize copyright laws, the author or authors    //
//   of this software dedicate any and all copyright interest in the          //
//   software to the public domain. We make this dedication for the benefit   //
//   of the public at large and to the detriment of our heirs and             //
//   successors. We intend this dedication to be an overt act of              //
//   relinquishment in perpetuity of all present and future rights to this    //
//   software under copyright law.                                            //
//                                                                            //
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          //
//   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       //
//   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   //
//   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR        //
//   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,    //
//   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR    //
//   OTHER DEALINGS IN THE SOFTWARE.                                          //
//                                                                            //
//   For more information, please refer to <https://unlicense.org>            //
////////////////////////////////////////////////////////////////////////////////
//    WOS : Web Operating System                                              //
//     System/SysWindow.cpp : System window management                        //
////////////////////////////////////////////////////////////////////////////////
#include "SysWindow.h"


////////////////////////////////////////////////////////////////////////////////
//  SysWindow global instance                                                 //
////////////////////////////////////////////////////////////////////////////////
SysWindow GSysWindow = SysWindow();


////////////////////////////////////////////////////////////////////////////////
//  Get window width                                                          //
////////////////////////////////////////////////////////////////////////////////
EM_JS(int, SysGetWindowWidth, (), {
    return window.innerWidth;
});

////////////////////////////////////////////////////////////////////////////////
//  Get window height                                                         //
////////////////////////////////////////////////////////////////////////////////
EM_JS(int, SysGetWindowHeight, (), {
    return window.innerHeight;
});


////////////////////////////////////////////////////////////////////////////////
//  Window resize callback function                                           //
////////////////////////////////////////////////////////////////////////////////
EM_BOOL OnWindowResize(int event, const EmscriptenUiEvent* uievent, void* user)
{
    // Update window size
    (void)event;
    (void)uievent;
    (void)user;
    GSysWindow.updateSize();
    return EM_TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//  Window mouse callback function                                            //
////////////////////////////////////////////////////////////////////////////////
EM_BOOL OnWindowMouse(int event, const EmscriptenMouseEvent* mouse, void* user)
{
    // Update window mouse
    (void)event;
    (void)user;
    GSysWindow.updateMouse(mouse->clientX, mouse->clientY, mouse->buttons);
    return EM_TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//  Window mouse wheel callback function                                      //
////////////////////////////////////////////////////////////////////////////////
EM_BOOL OnWindowMouseWheel(
    int event, const EmscriptenWheelEvent* mouse, void* user)
{
    // Update window mouse wheel
    (void)event;
    (void)user;
    if (mouse->deltaY != 0.0) { GSysWindow.updateMouseWheel(mouse->deltaY); }
    return EM_FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//  Window keyboard down callback function                                    //
////////////////////////////////////////////////////////////////////////////////
EM_BOOL OnWindowKeyDown(
    int event, const EmscriptenKeyboardEvent* key, void* user)
{
    // Update window keyboard down
    (void)event;
    (void)user;
    GSysWindow.updateKeyDown(key->key, key->location);
    return EM_FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//  Window keyboard up callback function                                      //
////////////////////////////////////////////////////////////////////////////////
EM_BOOL OnWindowKeyUp(
    int event, const EmscriptenKeyboardEvent* key, void* user)
{
    // Update window keyboard up
    (void)event;
    (void)user;
    GSysWindow.updateKeyUp(key->key, key->location);
    return EM_FALSE;
}


////////////////////////////////////////////////////////////////////////////////
//  SysWindow default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
SysWindow::SysWindow() :
m_handle(0),
m_mutex(),
m_width(1),
m_height(1),
m_mouseX(0),
m_mouseY(0),
m_buttons(0),
m_events()
{

}

////////////////////////////////////////////////////////////////////////////////
//  SysWindow destructor                                                      //
////////////////////////////////////////////////////////////////////////////////
SysWindow::~SysWindow()
{
    close();
}


////////////////////////////////////////////////////////////////////////////////
//  Create the window                                                         //
//  return : True if the window is successfully created                       //
////////////////////////////////////////////////////////////////////////////////
bool SysWindow::create()
{
    // Lock window context mutex
    m_mutex.lock();

    // Set window attributes
    EmscriptenWebGLContextAttributes attributes;
    emscripten_webgl_init_context_attributes(&attributes);
    attributes.alpha = EM_FALSE;
    attributes.depth = EM_TRUE;
    attributes.stencil = EM_FALSE;
    attributes.antialias = EM_TRUE;
    attributes.premultipliedAlpha = EM_TRUE;
    attributes.preserveDrawingBuffer = EM_FALSE;
    attributes.powerPreference = EM_WEBGL_POWER_PREFERENCE_HIGH_PERFORMANCE;
    attributes.failIfMajorPerformanceCaveat = EM_FALSE;
    attributes.majorVersion = 1;
    attributes.minorVersion = 0;
    attributes.enableExtensionsByDefault = EM_TRUE;
    attributes.explicitSwapControl = EM_FALSE;
    attributes.renderViaOffscreenBackBuffer = EM_FALSE;
    attributes.proxyContextToMainThread =
        EMSCRIPTEN_WEBGL_CONTEXT_PROXY_DISALLOW;

    // Init window
    m_handle = emscripten_webgl_create_context("#woscreen", &attributes);
    emscripten_webgl_make_context_current(m_handle);
    if (!m_handle)
    {
        // Invalid window handle
        m_mutex.unlock();
        return false;
    }

    // Check supported features
    if (!emscripten_supports_offscreencanvas())
    {
        // Offscreen canvas not supported
        m_mutex.unlock();
        return false;
    }

    // Update window size
    m_width = SysGetWindowWidth();
    m_height = SysGetWindowHeight();
    emscripten_set_canvas_element_size("#woscreen", m_width, m_height);

    // Set window resize callback
    emscripten_set_resize_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, EM_FALSE, OnWindowResize
    );

    // Set mouse events callbacks
    emscripten_set_mousemove_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, EM_FALSE, OnWindowMouse
    );
    emscripten_set_click_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, EM_FALSE, OnWindowMouse
    );
    emscripten_set_dblclick_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, EM_FALSE, OnWindowMouse
    );
    emscripten_set_mousedown_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, EM_FALSE, OnWindowMouse
    );
    emscripten_set_mouseup_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, EM_FALSE, OnWindowMouse
    );
    emscripten_set_wheel_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, EM_FALSE, OnWindowMouseWheel
    );

    // Set key events callbacks
    emscripten_set_keydown_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, EM_FALSE, OnWindowKeyDown
    );
    emscripten_set_keyup_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, EM_FALSE, OnWindowKeyUp
    );

    // System window successfully created
    m_mutex.unlock();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Close the window                                                          //
////////////////////////////////////////////////////////////////////////////////
void SysWindow::close()
{
    m_buttons = 0;
    m_mouseY = 0;
    m_mouseX = 0;
    if (m_handle)
    {
        // Destroy the window
        m_mutex.lock();
        emscripten_webgl_make_context_current(0);
        emscripten_webgl_destroy_context(m_handle);
        m_mutex.unlock();
    }
    m_handle = 0;
    m_height = 0;
    m_width = 0;
}

////////////////////////////////////////////////////////////////////////////////
//  Update window size                                                        //
////////////////////////////////////////////////////////////////////////////////
void SysWindow::updateSize()
{
    // Update window size
    m_width = SysGetWindowWidth();
    m_height = SysGetWindowHeight();
    emscripten_set_canvas_element_size("#woscreen", m_width, m_height);
}

////////////////////////////////////////////////////////////////////////////////
//  Update window mouse                                                       //
////////////////////////////////////////////////////////////////////////////////
void SysWindow::updateMouse(
    long int mouseX, long int mouseY, unsigned short buttons)
{
    Event event;
    event.type = EVENT_NONE;

    // Mouse move event
    if ((mouseX != m_mouseX) || (mouseY != m_mouseY))
    {
        m_mouseX = mouseX;
        m_mouseY = mouseY;
        event.type = EVENT_MOUSEMOVED;
        event.mouse.x = mouseX;
        event.mouse.y = mouseY;
        m_events.push(event);
    }

    // Mouse press event
    if ((buttons & 0x01) && !(m_buttons & 0x01))
    {
        // Left button pressed
        event.type = EVENT_MOUSEPRESSED;
        event.mouse.button = EVENT_MOUSE_LEFT;
        event.mouse.x = m_mouseX;
        event.mouse.y = m_mouseY;
        m_events.push(event);
    }
    if ((buttons & 0x02) && !(m_buttons & 0x02))
    {
        // Right button pressed
        event.type = EVENT_MOUSEPRESSED;
        event.mouse.button = EVENT_MOUSE_RIGHT;
        event.mouse.x = m_mouseX;
        event.mouse.y = m_mouseY;
        m_events.push(event);
    }
    if ((buttons & 0x04) && !(m_buttons & 0x04))
    {
        // Middle button pressed
        event.type = EVENT_MOUSEPRESSED;
        event.mouse.button = EVENT_MOUSE_MIDDLE;
        event.mouse.x = m_mouseX;
        event.mouse.y = m_mouseY;
        m_events.push(event);
    }

    // Mouse released event
    if (!(buttons & 0x01) && (m_buttons & 0x01))
    {
        // Left button released
        event.type = EVENT_MOUSERELEASED;
        event.mouse.button = EVENT_MOUSE_LEFT;
        event.mouse.x = m_mouseX;
        event.mouse.y = m_mouseY;
        m_events.push(event);
    }
    if (!(buttons & 0x02) && (m_buttons & 0x02))
    {
        // Right button released
        event.type = EVENT_MOUSERELEASED;
        event.mouse.button = EVENT_MOUSE_RIGHT;
        event.mouse.x = m_mouseX;
        event.mouse.y = m_mouseY;
        m_events.push(event);
    }
    if (!(buttons & 0x04) && (m_buttons & 0x04))
    {
        // Middle button released
        event.type = EVENT_MOUSERELEASED;
        event.mouse.button = EVENT_MOUSE_MIDDLE;
        event.mouse.x = m_mouseX;
        event.mouse.y = m_mouseY;
        m_events.push(event);
    }

    // Update mouse buttons states
    m_buttons = buttons;
}

////////////////////////////////////////////////////////////////////////////////
//  Update window mouse wheel                                                 //
////////////////////////////////////////////////////////////////////////////////
void SysWindow::updateMouseWheel(double delta)
{
    Event event;
    event.type = EVENT_NONE;

    // Mouse wheel event
    event.type = EVENT_MOUSEWHEEL;
    event.mouse.wheel = ((delta < 0.0) ? 127 : -127);
    m_events.push(event);
}

////////////////////////////////////////////////////////////////////////////////
//  Update window keyboard down                                               //
////////////////////////////////////////////////////////////////////////////////
void SysWindow::updateKeyDown(const EM_UTF8 key[32], unsigned long location)
{
    Event event;
    event.type = EVENT_NONE;

    // Key pressed event
    event.type = EVENT_KEYPRESSED;
    event.key = transcriptKey(key, location);
    m_events.push(event);
}

////////////////////////////////////////////////////////////////////////////////
//  Update window keyboard up                                                 //
////////////////////////////////////////////////////////////////////////////////
void SysWindow::updateKeyUp(const EM_UTF8 key[32], unsigned long location)
{
    Event event;
    event.type = EVENT_NONE;

    // Key released event
    event.type = EVENT_KEYRELEASED;
    event.key = transcriptKey(key, location);
    m_events.push(event);
}


////////////////////////////////////////////////////////////////////////////////
//  Get window event                                                          //
//  return : True if an event occurred, false otherwise                       //
////////////////////////////////////////////////////////////////////////////////
bool SysWindow::getEvent(Event& event)
{
    // Get event in the FIFO queue
    event.type = EVENT_NONE;
    if (!m_events.empty())
    {
        event = m_events.front();
        m_events.pop();
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////
//  Transcript key event                                                      //
////////////////////////////////////////////////////////////////////////////////
EventKey SysWindow::transcriptKey(
    const EM_UTF8 key[32], unsigned long location)
{
    switch (key[0])
    {
        case 69:
        {
            switch (key[1])
            {
                case 115: return EVENT_KEY_ESCAPE;
                case 110: return EVENT_KEY_RETURN;
                default: return EVENT_KEY_E;
            }
            return EVENT_KEY_E;
        }
        case 32: return EVENT_KEY_SPACE;
        case 66: return ((key[1] != 0) ? EVENT_KEY_BACKSPACE : EVENT_KEY_B);

        case 77:
        {
            if (key[1] != 0)
            {
                return ((location == DOM_KEY_LOCATION_LEFT) ?
                    EVENT_KEY_LSYS : EVENT_KEY_RSYS);
            }
            return EVENT_KEY_M;
        }
        case 67:
        {
            if (key[1] != 0)
            {
                return ((location == DOM_KEY_LOCATION_LEFT) ?
                    EVENT_KEY_LCTRL : EVENT_KEY_RCTRL);
            }
            return EVENT_KEY_C;
        }
        case 65:
        {
            if (key[1] != 0)
            {
                switch (key[5])
                {
                    case 85: return EVENT_KEY_UP;
                    case 68: return EVENT_KEY_DOWN;
                    case 76: return EVENT_KEY_LEFT;
                    case 82: return EVENT_KEY_RIGHT;
                    default: return ((location == DOM_KEY_LOCATION_LEFT) ?
                        EVENT_KEY_LALT : EVENT_KEY_RALT);
                }
                return EVENT_KEY_A;
            }
            return EVENT_KEY_A;
        }
        case 83:
        {
            if (key[1] != 0)
            {
                return ((location == DOM_KEY_LOCATION_LEFT) ?
                    EVENT_KEY_LSHIFT : EVENT_KEY_RSHIFT);
            }
            return EVENT_KEY_S;
        }
        case 84: return ((key[1] != 0) ? EVENT_KEY_TAB : EVENT_KEY_T);

        case 70:
        {
            switch (key[1])
            {
                case 49:
                {
                    switch (key[2])
                    {
                        case 48: return EVENT_KEY_F10;
                        case 49: return EVENT_KEY_F11;
                        case 50: return EVENT_KEY_F12;
                        default: return EVENT_KEY_F1;
                    }
                    return EVENT_KEY_F;
                }
                case 50: return EVENT_KEY_F2;
                case 51: return EVENT_KEY_F3;
                case 52: return EVENT_KEY_F4;
                case 53: return EVENT_KEY_F5;
                case 54: return EVENT_KEY_F6;
                case 55: return EVENT_KEY_F7;
                case 56: return EVENT_KEY_F8;
                case 57: return EVENT_KEY_F9;
                default: return EVENT_KEY_F;
            }
            return EVENT_KEY_F;
        }

        case 'a': return EVENT_KEY_A;
        case 'b': return EVENT_KEY_B;
        case 'c': return EVENT_KEY_C;
        case 'D': case 'd': return EVENT_KEY_D;
        case 'e': return EVENT_KEY_E;
        case 'f': return EVENT_KEY_F;
        case 'G': case 'g': return EVENT_KEY_G;
        case 'H': case 'h': return EVENT_KEY_H;
        case 'I': case 'i': return EVENT_KEY_I;
        case 'J': case 'j': return EVENT_KEY_J;
        case 'K': case 'k': return EVENT_KEY_K;
        case 'L': case 'l': return EVENT_KEY_L;
        case 'm': return EVENT_KEY_M;
        case 'N': case 'n': return EVENT_KEY_N;
        case 'O': case 'o': return EVENT_KEY_O;
        case 'P': case 'p': return EVENT_KEY_P;
        case 'Q': case 'q': return EVENT_KEY_Q;
        case 'R': case 'r': return EVENT_KEY_R;
        case 's': return EVENT_KEY_S;
        case 't': return EVENT_KEY_T;
        case 'U': case 'u': return EVENT_KEY_U;
        case 'V': case 'v': return EVENT_KEY_V;
        case 'W': case 'w': return EVENT_KEY_W;
        case 'X': case 'x': return EVENT_KEY_X;
        case 'Y': case 'y': return EVENT_KEY_Y;
        case 'Z': case 'z': return EVENT_KEY_Z;

        case '0': return EVENT_KEY_0;
        case '1': return EVENT_KEY_1;
        case '2': return EVENT_KEY_2;
        case '3': return EVENT_KEY_3;
        case '4': return EVENT_KEY_4;
        case '5': return EVENT_KEY_5;
        case '6': return EVENT_KEY_6;
        case '7': return EVENT_KEY_7;
        case '8': return EVENT_KEY_8;
        case '9': return EVENT_KEY_9;

        default: return EVENT_KEY_NONE;
    }
}
