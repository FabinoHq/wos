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
//     Renderer/Sprite.cpp : Sprite management                                //
////////////////////////////////////////////////////////////////////////////////
#include "Sprite.h"


////////////////////////////////////////////////////////////////////////////////
//  Sprite default constructor                                                //
////////////////////////////////////////////////////////////////////////////////
Sprite::Sprite() :
Transform2(),
m_texture(0),
m_color(1.0f, 1.0f, 1.0f, 1.0f),
m_uvOffset(0.0f, 0.0f),
m_uvSize(1.0f, 1.0f)
{

}

////////////////////////////////////////////////////////////////////////////////
//  Sprite virtual destructor                                                 //
////////////////////////////////////////////////////////////////////////////////
Sprite::~Sprite()
{
    m_uvSize.reset();
    m_uvOffset.reset();
    m_color.reset();
    m_texture = 0;
}


////////////////////////////////////////////////////////////////////////////////
//  Init sprite                                                               //
//  return : True if the sprite is successfully created                       //
////////////////////////////////////////////////////////////////////////////////
bool Sprite::init(Texture& texture, float width, float height)
{
    // Check texture handle
    if (!texture.isValid())
    {
        // Invalid texture handle
        return false;
    }

    // Reset sprite transformations
    resetTransforms();

    // Set sprite size
    setSize(width, height);

    // Center sprite origin (anchor)
    centerOrigin();

    // Set sprite texture pointer
    m_texture = &texture;

    // Reset sprite color
    m_color.set(1.0f, 1.0f, 1.0f, 1.0f);

    // Reset sprite UV offset
    m_uvOffset.reset();

    // Reset sprite UV size
    m_uvSize.set(1.0f, 1.0f);

    // Sprite successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Set sprite texture                                                        //
//  return : True if sprite texture is successfully set                       //
////////////////////////////////////////////////////////////////////////////////
bool Sprite::setTexture(Texture& texture)
{
    // Check texture handle
    if (!texture.isValid())
    {
        // Invalid texture handle
        return false;
    }

    // Set sprite texture pointer
    m_texture = &texture;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Set sprite color                                                          //
////////////////////////////////////////////////////////////////////////////////
void Sprite::setColor(const Vector4& color)
{
    m_color.vec[0] = color.vec[0];
    m_color.vec[1] = color.vec[1];
    m_color.vec[2] = color.vec[2];
    m_color.vec[3] = color.vec[3];
}

////////////////////////////////////////////////////////////////////////////////
//  Set sprite color                                                          //
////////////////////////////////////////////////////////////////////////////////
void Sprite::setColor(float red, float green, float blue, float alpha)
{
    m_color.vec[0] = red;
    m_color.vec[1] = green;
    m_color.vec[2] = blue;
    m_color.vec[3] = alpha;
}


////////////////////////////////////////////////////////////////////////////////
//  Set sprite subrectangle                                                   //
////////////////////////////////////////////////////////////////////////////////
void Sprite::setSubrect(const Vector2& offset, const Vector2& size)
{
    m_uvOffset.vec[0] = offset.vec[0];
    m_uvOffset.vec[1] = offset.vec[1];
    m_uvSize.vec[0] = size.vec[0];
    m_uvSize.vec[1] = size.vec[1];
}

////////////////////////////////////////////////////////////////////////////////
//  Set sprite subrectangle                                                   //
////////////////////////////////////////////////////////////////////////////////
void Sprite::setSubrect(float uOffset, float vOffset, float uSize, float vSize)
{
    m_uvOffset.vec[0] = uOffset;
    m_uvOffset.vec[1] = vOffset;
    m_uvSize.vec[0] = uSize;
    m_uvSize.vec[1] = vSize;
}


////////////////////////////////////////////////////////////////////////////////
//  Render sprite                                                             //
////////////////////////////////////////////////////////////////////////////////
void Sprite::render()
{
    // Compute sprite transformations
    computeTransforms();

    // Upload model matrix
    GRenderer.currentShader->sendModelMatrix(m_matrix);

    // Render sprite
    GRenderer.currentBuffer->render();
}
