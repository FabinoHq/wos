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
//     System/SysMessage.h : System Message management                        //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_SYSTEM_SYSMESSAGE_HEADER
#define WOS_SYSTEM_SYSMESSAGE_HEADER

    #include "System.h"
    #include "SysMutex.h"

    #include <iostream>
    #include <string>
    #include <sstream>


    ////////////////////////////////////////////////////////////////////////////
    //  SysMessage class definition                                           //
    ////////////////////////////////////////////////////////////////////////////
    class SysMessage
    {
        private:
            ////////////////////////////////////////////////////////////////////
            //  SysMessage private constructor                                //
            ////////////////////////////////////////////////////////////////////
            SysMessage();

        public:
            ////////////////////////////////////////////////////////////////////
            //  Get the system message global singleton instance              //
            //  return : SysMessage singleton instance                        //
            ////////////////////////////////////////////////////////////////////
            static SysMessage& box();

            ////////////////////////////////////////////////////////////////////
            //  Display the system message                                    //
            ////////////////////////////////////////////////////////////////////
            void display();

            ////////////////////////////////////////////////////////////////////
            //  Add a system message                                          //
            //  return : Reference to the SysMessage instance                 //
            ////////////////////////////////////////////////////////////////////
            template<typename T> SysMessage& operator<<(const T& t)
            {
                // Add to system message
                m_mutex.lock();
                m_message << t;
                m_display = true;
                m_mutex.unlock();
                return *this;
            }


        private:
            ////////////////////////////////////////////////////////////////////
            //  SysMessage private copy constructor : Not copyable            //
            ////////////////////////////////////////////////////////////////////
            SysMessage(const SysMessage&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  SysMessage private copy operator : Not copyable               //
            ////////////////////////////////////////////////////////////////////
            SysMessage& operator=(const SysMessage&) = delete;


        private:
            SysMutex                m_mutex;    // System message mutex
            bool                    m_display;  // Display the system message
            std::ostringstream      m_message;  // Message to display
    };


#endif // WOS_SYSTEM_SYSMESSAGE_HEADER
