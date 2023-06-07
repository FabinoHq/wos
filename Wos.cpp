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
//  WOS global instance                                                       //
////////////////////////////////////////////////////////////////////////////////
Wos GWos = Wos();


////////////////////////////////////////////////////////////////////////////////
//  WOS main loop callback function                                           //
////////////////////////////////////////////////////////////////////////////////
void WosMainLoop()
{
    GWos.run();
}


////////////////////////////////////////////////////////////////////////////////
//  Wos default constructor                                                   //
////////////////////////////////////////////////////////////////////////////////
Wos::Wos() :
m_state(WOS_STATE_NONE),
m_clock(),
m_frametime(0.0f),
m_framecount(0.0f),
m_framerate(0.0f),
m_timer(0.0f),
m_game()
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

    // Init system settings
    if (!GSysSettings.loadSettings())
    {
        // Unable to load system settings
        return false;
    }

    // Init WOS renderer
    if (!GRenderer.init())
    {
        // Unable to init WOS renderer
        return false;
    }

    // Run WOS
    m_clock.reset();
    emscripten_set_main_loop(WosMainLoop, 0, 1);

    // WOS successfully terminated
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Run WOS                                                                   //
////////////////////////////////////////////////////////////////////////////////
void Wos::run()
{
    // Compute average framerate
    float frametime = m_clock.getAndResetF();
    m_timer += frametime;
    m_frametime += frametime;
    m_framecount += 1.0f;
    if (m_framecount >= 30.0f)
    {
        m_frametime /= m_framecount;
        if (m_frametime > 0.0f) { m_framerate = 1.0f/m_frametime; }
        m_framecount = 0.0f;
        m_frametime = 0.0f;
    }

    switch (m_state)
    {
        case WOS_STATE_NONE:
            // Boot to init state
            m_timer = 0.0f;
            m_state = WOS_STATE_INIT;
            break;

        case WOS_STATE_INIT:
            // Init resources
            GResources.init();
            m_state = WOS_STATE_PRELOAD;
            break;

        case WOS_STATE_PRELOAD:
            if (m_timer >= ResourcesWaitSleepTime)
            {
                if (GResources.isInitDone())
                {
                    // Preload resources
                    GResources.preload();
                    m_state = WOS_STATE_LOAD;
                }
                m_timer = 0.0f;
            }
            break;

        case WOS_STATE_LOAD:
            if (m_timer >= ResourcesWaitSleepTime)
            {
                if (GResources.isInitDone())
                {
                    // Load resources
                    GResources.startLoading();
                    m_state = WOS_STATE_WAIT;
                }
                m_timer = 0.0f;
            }
            break;

        case WOS_STATE_WAIT:
            if (m_timer >= ResourcesWaitSleepTime)
            {
                if (GResources.isLoadingDone())
                {
                    // Init game
                    if (m_game.init())
                    {
                        // Run WOS
                        m_state = WOS_STATE_RUN;
                    }
                    else
                    {
                        // Could not init game
                        m_state = WOS_STATE_ERROR;
                    }
                }
                m_timer = 0.0f;
            }
            break;

        case WOS_STATE_RUN:
        {
            // Get main window event
            Event event;
            while (GSysWindow.getEvent(event))
            {
                // Compute events
                m_game.events(event);
            }

            // Compute logic
            m_game.compute(frametime);

            // Render frame
            m_game.render();
            break;
        }

        default:
            // Invalid state
            break;
    }
}
