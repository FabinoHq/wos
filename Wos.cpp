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
//     Wos.cpp : WOS Main class management                                    //
////////////////////////////////////////////////////////////////////////////////
#include "Wos.h"


////////////////////////////////////////////////////////////////////////////////
//  Wos default constructor                                                   //
////////////////////////////////////////////////////////////////////////////////
Wos::Wos() :
m_running(false),
m_clock()
{

}

////////////////////////////////////////////////////////////////////////////////
//  Wos destructor                                                            //
////////////////////////////////////////////////////////////////////////////////
Wos::~Wos()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Launch WOS                                                                //
//  return : True if WOS successfully started, false otherwise                //
////////////////////////////////////////////////////////////////////////////////
bool Wos::launch()
{
    // Check system CPU
    if (!SysCPUCheck())
    {
        // Invalid system CPU
        return false;
    }

    // Create WOS global window
    if (!GSysWindow.create())
    {
        // Unable to create WOS global window
        return false;
    }

    // Init VWOS renderer
    if (!GRenderer.init())
    {
        // Unable to init WOS renderer
        return false;
    }

    // Run WOS
    run();

    // WOS successfully terminated
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Run WOS                                                                   //
////////////////////////////////////////////////////////////////////////////////
void Wos::run()
{
    // Run WOS
    m_clock.reset();
    m_running = true;
    while (m_running)
    {
        std::cout << "Running\n";
        SysSleep(0.1);
    }
}
