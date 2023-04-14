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
//     Renderer/ProcSprite.cpp : Procedural sprite management                 //
////////////////////////////////////////////////////////////////////////////////
#include "ProcSprite.h"


////////////////////////////////////////////////////////////////////////////////
//  ProcSprite default constructor                                            //
////////////////////////////////////////////////////////////////////////////////
ProcSprite::ProcSprite() :
Transform2(),
m_shader(),
m_color(1.0f, 1.0f, 1.0f, 1.0f)
{

}

////////////////////////////////////////////////////////////////////////////////
//  ProcSprite virtual destructor                                             //
////////////////////////////////////////////////////////////////////////////////
ProcSprite::~ProcSprite()
{
    m_color.reset();
}


////////////////////////////////////////////////////////////////////////////////
//  Init procedural sprite                                                    //
//  return : True if the proc sprite is successfully created                  //
////////////////////////////////////////////////////////////////////////////////
bool ProcSprite::init(float width, float height, const char* fragmentSource)
{
    bool shaderCreated = false;
    if (fragmentSource)
    {
        // Create procedural sprite shader
        if (m_shader.createShader(DefaultProcVertexShaderSrc, fragmentSource))
        {
            shaderCreated = true;
        }
    }

    if (!shaderCreated)
    {
        // Create default procedural sprite shader
        if (!m_shader.createShader(
            DefaultProcVertexShaderSrc, DefaultProcFragmentShaderSrc))
        {
            // Could not create default procedural sprite shader
            return false;
        }
    }

    // Reset procedural sprite transformations
    resetTransforms();

    // Set procedural sprite size
    setSize(width, height);

    // Center procedural sprite origin (anchor)
    centerOrigin();

    // Reset procedural sprite color
    m_color.set(1.0f, 1.0f, 1.0f, 1.0f);

    // Procedural sprite successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy procedural sprite                                                 //
////////////////////////////////////////////////////////////////////////////////
void ProcSprite::destroyProcSprite()
{
    m_color.reset();
    resetTransforms();
}

////////////////////////////////////////////////////////////////////////////////
//  Set procedural sprite color                                               //
////////////////////////////////////////////////////////////////////////////////
void ProcSprite::setColor(const Vector4& color)
{
    m_color.vec[0] = color.vec[0];
    m_color.vec[1] = color.vec[1];
    m_color.vec[2] = color.vec[2];
    m_color.vec[3] = color.vec[3];
}

////////////////////////////////////////////////////////////////////////////////
//  Set procedural sprite color                                               //
////////////////////////////////////////////////////////////////////////////////
void ProcSprite::setColor(float red, float green, float blue, float alpha)
{
    m_color.vec[0] = red;
    m_color.vec[1] = green;
    m_color.vec[2] = blue;
    m_color.vec[3] = alpha;
}


////////////////////////////////////////////////////////////////////////////////
//  Render procedural sprite                                                  //
////////////////////////////////////////////////////////////////////////////////
void ProcSprite::render()
{
    // Compute sprite transformations
    computeTransforms();
}
