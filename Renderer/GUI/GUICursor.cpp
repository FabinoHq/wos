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
//     Renderer/GUI/GUICursor.cpp : GUI Cursor management                     //
////////////////////////////////////////////////////////////////////////////////
#include "GUICursor.h"
#include "../Renderer.h"


////////////////////////////////////////////////////////////////////////////////
//  GUICursor default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
GUICursor::GUICursor() :
Transform2(),
m_texture(0),
m_color(1.0f, 1.0f, 1.0f, 1.0f),
m_offset(0.0f, 0.0f),
m_scale(1.0f)
{

}

////////////////////////////////////////////////////////////////////////////////
//  GUICursor virtual destructor                                              //
////////////////////////////////////////////////////////////////////////////////
GUICursor::~GUICursor()
{
    m_scale = 1.0f;
    m_offset.reset();
	m_color.reset();
    m_texture = 0;
}


////////////////////////////////////////////////////////////////////////////////
//  Init cursor                                                               //
//  return : True if the cursor is successfully created                       //
////////////////////////////////////////////////////////////////////////////////
bool GUICursor::init(Texture& texture, float scale)
{
    // Check texture handle
    if (!texture.isValid())
    {
        // Invalid texture handle
        return false;
    }

    // Reset cursor transformations
    resetTransforms();

    // Set cursor texture pointer
    m_texture = &texture;

    // Reset cursor color
    m_color.set(1.0f, 1.0f, 1.0f, 1.0f);

    // Set cursor scale
    m_scale = scale;

    // Set cursor offset
    m_offset = GUICusorDefaultOffset;

    // Cursor successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Set cursor                                                                //
////////////////////////////////////////////////////////////////////////////////
void GUICursor::setCursor(SysCursorType cursorType)
{
    switch (cursorType)
    {
        case SYSCURSOR_TOPRESIZE: case SYSCURSOR_BOTTOMRESIZE:
            m_texture = &GResources.textures.gui(TEXTURE_NSCURSOR);
            m_offset = GUICusorNSCursorOffset;
            break;

        case SYSCURSOR_LEFTRESIZE: case SYSCURSOR_RIGHTRESIZE:
            m_texture = &GResources.textures.gui(TEXTURE_EWCURSOR);
            m_offset = GUICusorEWCursorOffset;
            break;

        case SYSCURSOR_TOPRIGHTRESIZE: case SYSCURSOR_BOTTOMLEFTRESIZE:
            m_texture = &GResources.textures.gui(TEXTURE_NESWCURSOR);
            m_offset = GUICusorNESWCursorOffset;
            break;

        case SYSCURSOR_TOPLEFTRESIZE: case SYSCURSOR_BOTTOMRIGHTRESIZE:
            m_texture = &GResources.textures.gui(TEXTURE_NWSECURSOR);
            m_offset = GUICusorNWSECursorOffset;
            break;

        default:
            m_texture = &GResources.textures.gui(TEXTURE_CURSOR);
            m_offset = GUICusorDefaultOffset;
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Set cursor texture                                                        //
//  return : True if cursor texture is successfully set                       //
////////////////////////////////////////////////////////////////////////////////
bool GUICursor::setTexture(Texture& texture)
{
    // Check texture handle
    if (!texture.isValid())
    {
        // Invalid texture handle
        return false;
    }

    // Set cursor texture pointer
    m_texture = &texture;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Set cursor color                                                          //
////////////////////////////////////////////////////////////////////////////////
void GUICursor::setColor(const Vector4& color)
{
    m_color.vec[0] = color.vec[0];
    m_color.vec[1] = color.vec[1];
    m_color.vec[2] = color.vec[2];
    m_color.vec[3] = color.vec[3];
}

////////////////////////////////////////////////////////////////////////////////
//  Set cursor color                                                          //
////////////////////////////////////////////////////////////////////////////////
void GUICursor::setColor(float red, float green, float blue, float alpha)
{
    m_color.vec[0] = red;
    m_color.vec[1] = green;
    m_color.vec[2] = blue;
    m_color.vec[3] = alpha;
}


////////////////////////////////////////////////////////////////////////////////
//  Render cursor                                                             //
////////////////////////////////////////////////////////////////////////////////
void GUICursor::render()
{
    // Compute cursor transformations
    float scale = GRenderer.getScale();
    float cursorSize = (scale*m_scale);
    setSize(cursorSize, cursorSize);
    scale *= 2.0f;
    setOrigin((m_offset.vec[0]*scale), (m_offset.vec[1]*scale));
    computeTransforms();

    // Upload model matrix
    GRenderer.currentShader->sendModelMatrix(m_matrix);

    // Send uniforms constants
    GRenderer.currentShader->sendColor(m_color);
    GRenderer.currentShader->sendOffset(Vector2(0.0f, 0.0f));
    GRenderer.currentShader->sendSize(Vector2(1.0f, 1.0f));

    // Render cursor
    GRenderer.currentBuffer->render();
}
