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
//     System/SysMouse.cpp : System Mouse management                          //
////////////////////////////////////////////////////////////////////////////////
#include "SysMouse.h"
#include "../Renderer/Renderer.h"


////////////////////////////////////////////////////////////////////////////////
//  SysMouse global instance                                                  //
////////////////////////////////////////////////////////////////////////////////
SysMouse GSysMouse = SysMouse();


////////////////////////////////////////////////////////////////////////////////
//  SysMouse default constructor                                              //
////////////////////////////////////////////////////////////////////////////////
SysMouse::SysMouse() :
mouseX(0.0f),
mouseY(0.0f),
deltaX(0.0f),
deltaY(0.0f),
previousX(0),
previousY(0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  SysMouse destructor                                                       //
////////////////////////////////////////////////////////////////////////////////
SysMouse::~SysMouse()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Update mouse position                                                     //
////////////////////////////////////////////////////////////////////////////////
void SysMouse::update(int x, int y)
{
    // Get renderer scale and ratio
    float scale = GRenderer.getScale();
    float ratio = GRenderer.getRatio();

    // Compute mouse position
    #if (WOS_POINTERLOCK == 1)
        // High precision mouse delta
        deltaX = (x*1.0f);
        deltaY = (y*1.0f);
        mouseX += (x*scale*2.0f);
        mouseY -= (y*scale*2.0f);
        if (mouseX <= -ratio) { mouseX = -ratio; }
        if (mouseX >= ratio) { mouseX = ratio; }
        if (mouseY <= -1.0f) { mouseY = -1.0f; }
        if (mouseY >= 1.0f) { mouseY = 1.0f; }
    #else
        // OS absolute mouse position
        deltaX = ((x-previousX)*1.0f);
        deltaY = ((y-previousY)*1.0f);
        mouseX = (
            ((x-GRenderer.getOffsetXF())/
            (GRenderer.getWidthF()*0.5f))*ratio
        )-ratio;
        mouseY = (
            -(y-GRenderer.getOffsetYF())/
            (GRenderer.getHeightF()*0.5f)
        )+1.0f;
        if (mouseX <= -ratio) { mouseX = -ratio; }
        if (mouseX >= ratio) { mouseX = ratio; }
        if (mouseY <= -1.0f) { mouseY = -1.0f; }
        if (mouseY >= 1.0f) { mouseY = 1.0f; }
    #endif // WOS_POINTERLOCK

    // Update previous mouse position
    previousX = x;
    previousY = y;
}
