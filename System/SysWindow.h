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
    #include "SysMutex.h"
    #include "SysSleep.h"
    #include "SysMessage.h"
    #include "SysEvent.h"

    #include <queue>


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
            //  Close the window                                              //
            ////////////////////////////////////////////////////////////////////
            void close();

            ////////////////////////////////////////////////////////////////////
            //  Update window size                                            //
            ////////////////////////////////////////////////////////////////////
            void updateSize();

            ////////////////////////////////////////////////////////////////////
            //  Update window mouse                                           //
            ////////////////////////////////////////////////////////////////////
            void updateMouse(
                long int mouseX, long int mouseY, unsigned short buttons);


            ////////////////////////////////////////////////////////////////////
            //  Set current thread as current context                         //
            ////////////////////////////////////////////////////////////////////
            inline void setThread()
            {
                m_mutex.lock();
                emscripten_webgl_make_context_current(m_handle);
            }

            ////////////////////////////////////////////////////////////////////
            //  Release current thread from current context                   //
            ////////////////////////////////////////////////////////////////////
            inline void releaseThread()
            {
                emscripten_webgl_make_context_current(0);
                m_mutex.unlock();
            }


            ////////////////////////////////////////////////////////////////////
            //  Get window handle                                             //
            //  return : Reference to the window handle                       //
            ////////////////////////////////////////////////////////////////////
            inline EMSCRIPTEN_WEBGL_CONTEXT_HANDLE& getHandle()
            {
                return m_handle;
            }

            ////////////////////////////////////////////////////////////////////
            //  Check if the window has a valid handle                        //
            //  return : True if the window is valid                          //
            ////////////////////////////////////////////////////////////////////
            inline bool isValid() const
            {
                return m_handle;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get window width                                              //
            //  return : Window width                                         //
            ////////////////////////////////////////////////////////////////////
            inline int getWidth()
            {
                return m_width;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get window height                                             //
            //  return : Window height                                        //
            ////////////////////////////////////////////////////////////////////
            inline int getHeight()
            {
                return m_height;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get window event                                              //
            //  return : True if an event occurred, false otherwise           //
            ////////////////////////////////////////////////////////////////////
            bool getEvent(Event& event);


        private:
            ////////////////////////////////////////////////////////////////////
            //  SysWindow private copy constructor : Not copyable             //
            ////////////////////////////////////////////////////////////////////
            SysWindow(const SysWindow&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  SysWindow private copy operator : Not copyable                //
            ////////////////////////////////////////////////////////////////////
            SysWindow& operator=(const SysWindow&) = delete;


            ////////////////////////////////////////////////////////////////////
            //  Transcript key event                                          //
            ////////////////////////////////////////////////////////////////////
            EventKey transcriptKey();


        private:
            EMSCRIPTEN_WEBGL_CONTEXT_HANDLE     m_handle;       // Handle
            SysMutex                            m_mutex;        // Mutex

            int                                 m_width;        // Width
            int                                 m_height;       // Height

            long int                            m_mouseX;       // Mouse X
            long int                            m_mouseY;       // Mouse Y
            unsigned short                      m_buttons;      // Mouse buttons

            std::queue<Event>   m_events;       // Events FIFO queue
    };


    ////////////////////////////////////////////////////////////////////////////
    //  SysWindow global instance                                             //
    ////////////////////////////////////////////////////////////////////////////
    extern SysWindow GSysWindow;


#endif // WOS_SYSTEM_SYSWINDOW_HEADER
