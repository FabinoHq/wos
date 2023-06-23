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
//     System/SysCursor.h : System Cursor management                          //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_SYSTEM_SYSCURSOR_HEADER
#define WOS_SYSTEM_SYSCURSOR_HEADER

    #include "System.h"


    ////////////////////////////////////////////////////////////////////////////
    //  SysCursor type enumeration                                            //
    ////////////////////////////////////////////////////////////////////////////
    enum SysCursorType
    {
        SYSCURSOR_NONE = 0,
        SYSCURSOR_DEFAULT = 1,

        SYSCURSOR_TOPRESIZE = 2,
        SYSCURSOR_BOTTOMRESIZE = 3,
        SYSCURSOR_LEFTRESIZE = 4,
        SYSCURSOR_RIGHTRESIZE = 5,
        SYSCURSOR_TOPLEFTRESIZE = 6,
        SYSCURSOR_TOPRIGHTRESIZE = 7,
        SYSCURSOR_BOTTOMLEFTRESIZE = 8,
        SYSCURSOR_BOTTOMRIGHTRESIZE = 9,

        SYSCURSOR_CURSORSCOUNT = 10
    };

    ////////////////////////////////////////////////////////////////////////////
    //  SysCursors                                                            //
    ////////////////////////////////////////////////////////////////////////////
    const char* const SysCursors[] = {
        "auto",             // SYSCURSOR_NONE
        "auto",             // SYSCURSOR_DEFAULT
        "ns-resize",        // SYSCURSOR_TOPRESIZE
        "ns-resize",        // SYSCURSOR_BOTTOMRESIZE
        "ew-resize",        // SYSCURSOR_LEFTRESIZE
        "ew-resize",        // SYSCURSOR_RIGHTRESIZE
        "nwse-resize",      // SYSCURSOR_TOPLEFTRESIZE
        "nesw-resize",      // SYSCURSOR_TOPRIGHTRESIZE
        "nesw-resize",      // SYSCURSOR_BOTTOMLEFTRESIZE
        "nwse-resize"       // SYSCURSOR_BOTTOMRIGHTRESIZE
    };


#endif // WOS_SYSTEM_SYSCURSOR_HEADER
