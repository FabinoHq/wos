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
//     Wos.h : WOS Main class management                                      //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_WOS_HEADER
#define WOS_WOS_HEADER

    #include <emscripten/html5.h>

    #include "System/System.h"
    #include "System/SysMessage.h"
    #include "System/SysCPU.h"
    #include "System/SysClock.h"
    #include "System/SysSleep.h"
    #include "System/SysWindow.h"

    #include "Renderer/Renderer.h"

    #include <cstddef>
    #include <cstdint>
    #include <new>


    ////////////////////////////////////////////////////////////////////////////
    //  WOS main loop callback function                                       //
    ////////////////////////////////////////////////////////////////////////////
    void WosMainLoop();


    ////////////////////////////////////////////////////////////////////////////
    //  WOS main class definition                                             //
    ////////////////////////////////////////////////////////////////////////////
    class Wos
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Wos default constructor                                       //
            ////////////////////////////////////////////////////////////////////
            Wos();

            ////////////////////////////////////////////////////////////////////
            //  Wos destructor                                                //
            ////////////////////////////////////////////////////////////////////
            ~Wos();


            ////////////////////////////////////////////////////////////////////
            //  Launch WOS                                                    //
            //  return : True if WOS successfully started, false otherwise    //
            ////////////////////////////////////////////////////////////////////
            bool launch();

            ////////////////////////////////////////////////////////////////////
            //  Run WOS                                                       //
            ////////////////////////////////////////////////////////////////////
            void run();


        private:
            ////////////////////////////////////////////////////////////////////
            //  Wos private copy constructor : Not copyable                   //
            ////////////////////////////////////////////////////////////////////
            Wos(const Wos&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Wos private copy operator : Not copyable                      //
            ////////////////////////////////////////////////////////////////////
            Wos& operator=(const Wos&) = delete;


        private:
            bool            m_running;          // WOS running state
            SysClock        m_clock;            // WOS clock
            float           m_frametime;        // WOS frametime
            float           m_framecount;       // WOS framecount
            float           m_framerate;        // WOS framerate
    };


    ////////////////////////////////////////////////////////////////////////////
    //  WOS global instance                                                   //
    ////////////////////////////////////////////////////////////////////////////
    extern Wos GWos;


#endif // WOS_WOS_HEADER
