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
//     System/SysWindow.h : System window management                          //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_SYSTEM_SYSWINDOW_HEADER
#define WOS_SYSTEM_SYSWINDOW_HEADER

    #include <emscripten/html5.h>

    #include "System.h"
    #include "SysSleep.h"
    #include "SysMessage.h"


    ////////////////////////////////////////////////////////////////////////////
    //  SysWindow class definition                                            //
    ////////////////////////////////////////////////////////////////////////////
    class SysWindow
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  SysWindow default constructor                                 //
            ////////////////////////////////////////////////////////////////////
            SysWindow();

            ////////////////////////////////////////////////////////////////////
            //  SysWindow destructor                                          //
            ////////////////////////////////////////////////////////////////////
            ~SysWindow();


            ////////////////////////////////////////////////////////////////////
            //  Create the window                                             //
            //  return : True if the window is successfully created           //
            ////////////////////////////////////////////////////////////////////
            bool create();


            ////////////////////////////////////////////////////////////////////
            //  Set window size                                               //
            ////////////////////////////////////////////////////////////////////
            inline void setSize(int width, int height)
            {
                emscripten_set_canvas_element_size("#woscreen", width, height);
            }

            ////////////////////////////////////////////////////////////////////
            //  Get window width                                              //
            //  return : Window width                                         //
            ////////////////////////////////////////////////////////////////////
            int getWidth();

            ////////////////////////////////////////////////////////////////////
            //  Get window height                                             //
            //  return : Window height                                        //
            ////////////////////////////////////////////////////////////////////
            int getHeight();


        private:
            ////////////////////////////////////////////////////////////////////
            //  SysWindow private copy constructor : Not copyable             //
            ////////////////////////////////////////////////////////////////////
            SysWindow(const SysWindow&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  SysWindow private copy operator : Not copyable                //
            ////////////////////////////////////////////////////////////////////
            SysWindow& operator=(const SysWindow&) = delete;


        private:
            EMSCRIPTEN_WEBGL_CONTEXT_HANDLE     m_handle;       // Handle
    };


    ////////////////////////////////////////////////////////////////////////////
    //  SysWindow global instance                                             //
    ////////////////////////////////////////////////////////////////////////////
    extern SysWindow GSysWindow;


#endif // WOS_SYSTEM_SYSWINDOW_HEADER
