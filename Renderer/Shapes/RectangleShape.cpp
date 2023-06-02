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
//     Renderer/Shapes/RectangleShape.cpp : Rectangle shape management        //
////////////////////////////////////////////////////////////////////////////////
#include "RectangleShape.h"
#include "../Renderer.h"


////////////////////////////////////////////////////////////////////////////////
//  RectangleShape default constructor                                        //
////////////////////////////////////////////////////////////////////////////////
RectangleShape::RectangleShape() :
Transform2(),
m_color(1.0f, 1.0f, 1.0f, 1.0f),
m_smooth(0.005f)
{

}

////////////////////////////////////////////////////////////////////////////////
//  RectangleShape virtual destructor                                         //
////////////////////////////////////////////////////////////////////////////////
RectangleShape::~RectangleShape()
{
    m_smooth = 0.0f;
    m_color.reset();
}


////////////////////////////////////////////////////////////////////////////////
//  Init rectangle                                                            //
//  return : True if the rectangle is successfully created                    //
////////////////////////////////////////////////////////////////////////////////
bool RectangleShape::init(float width, float height)
{
    // Reset rectangle transformations
    resetTransforms();

    // Set rectangle size
    setSize(width, height);

    // Center rectangle origin (anchor)
    centerOrigin();

    // Reset rectangle color
    m_color.set(1.0f, 1.0f, 1.0f, 1.0f);

    // Reset rectangle smooth amount
    m_smooth = 0.005f;

    // Rectangle successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Set rectangle color                                                       //
////////////////////////////////////////////////////////////////////////////////
void RectangleShape::setColor(const Vector4& color)
{
    m_color.vec[0] = color.vec[0];
    m_color.vec[1] = color.vec[1];
    m_color.vec[2] = color.vec[2];
    m_color.vec[3] = color.vec[3];
}

////////////////////////////////////////////////////////////////////////////////
//  Set rectangle color                                                       //
////////////////////////////////////////////////////////////////////////////////
void RectangleShape::setColor(float red, float green, float blue, float alpha)
{
    m_color.vec[0] = red;
    m_color.vec[1] = green;
    m_color.vec[2] = blue;
    m_color.vec[3] = alpha;
}


////////////////////////////////////////////////////////////////////////////////
//  Render rectangle                                                          //
////////////////////////////////////////////////////////////////////////////////
void RectangleShape::render()
{
    // Compute rectangle transformations
    computeTransforms();

    // Upload model matrix
    GRenderer.currentShader->sendModelMatrix(m_matrix);

    // Send uniforms constants
    GRenderer.currentShader->sendColor(m_color);
    GRenderer.currentShader->sendTime(m_smooth);

    // Render rectangle shape
    GRenderer.vertexBuffer.render();
}
