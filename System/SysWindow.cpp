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
//  SysWindow default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
SysWindow::SysWindow() :
m_handle(),
m_width(0),
m_height(0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  SysWindow destructor                                                      //
////////////////////////////////////////////////////////////////////////////////
SysWindow::~SysWindow()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Create the window                                                         //
//  return : True if the window is successfully created                       //
////////////////////////////////////////////////////////////////////////////////
bool SysWindow::create()
{
    // Set window attributes
    EmscriptenWebGLContextAttributes attributes;
    emscripten_webgl_init_context_attributes(&attributes);
    attributes.alpha = 0;
    attributes.depth = 0;
    attributes.stencil = 0;
    attributes.antialias = 0;
    attributes.preserveDrawingBuffer = 0;
    attributes.failIfMajorPerformanceCaveat = 0;

    // Init window
    m_handle = emscripten_webgl_create_context("#woscreen", &attributes);
    emscripten_webgl_make_context_current(m_handle);

    // Window size
    m_width = 0;
    m_height = 0;

    // System window successfully created
    return true;
}
