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
//  SysWindow default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
SysWindow::SysWindow() :
m_handle(0),
m_mutex(),
m_width(1),
m_height(1)
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
    attributes.depth = EM_FALSE;
    attributes.stencil = EM_FALSE;
    attributes.antialias = EM_FALSE;
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

    // System window successfully created
    m_mutex.unlock();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Close the window                                                          //
////////////////////////////////////////////////////////////////////////////////
void SysWindow::close()
{
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
//  Update window                                                             //
////////////////////////////////////////////////////////////////////////////////
void SysWindow::update()
{
    // Update window size
    m_width = SysGetWindowWidth();
    m_height = SysGetWindowHeight();
    emscripten_set_canvas_element_size("#woscreen", m_width, m_height);
}
