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
//     System/SysMutexLocker.h : System Mutex locker management               //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_SYSTEM_SYSMUTEXLOCKER_HEADER
#define WOS_SYSTEM_SYSMUTEXLOCKER_HEADER

    #include "System.h"
    #include "SysMutex.h"

    #include <thread>
    #include <mutex>


    ////////////////////////////////////////////////////////////////////////////
    //  SysMutexLocker class definition                                       //
    ////////////////////////////////////////////////////////////////////////////
    class SysMutexLocker
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  SysMutexLocker constructor : Lock the associated mutex        //
            ////////////////////////////////////////////////////////////////////
            inline SysMutexLocker(SysMutex& mutex) :
            m_mutex(mutex)
            {
                m_mutex.lock();
            }

            ////////////////////////////////////////////////////////////////////
            //  SysMutexLocker destructor : Unlock the associated mutex       //
            ////////////////////////////////////////////////////////////////////
            inline ~SysMutexLocker()
            {
                m_mutex.unlock();
            }


        private:
            ////////////////////////////////////////////////////////////////////
            //  SysMutexLocker private copy constructor : Not copyable        //
            ////////////////////////////////////////////////////////////////////
            SysMutexLocker(const SysMutexLocker&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  SysMutexLocker private copy operator : Not copyable           //
            ////////////////////////////////////////////////////////////////////
            SysMutexLocker& operator=(const SysMutexLocker&) = delete;


        private:
            SysMutex&           m_mutex;            // Associated mutex
    };


#endif // WOS_SYSTEM_SYSMUTEXLOCKER_HEADER
