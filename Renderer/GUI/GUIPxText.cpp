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
//     Renderer/GUI/GUIPxText.cpp : GUI PixelText (distance field) management //
////////////////////////////////////////////////////////////////////////////////
#include "GUIPxText.h"
#include "../Renderer.h"


////////////////////////////////////////////////////////////////////////////////
//  GUIPxText default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
GUIPxText::GUIPxText() :
Transform2(),
m_texture(0),
m_color(1.0f, 1.0f, 1.0f, 1.0f),
m_smooth(0.1f),
m_text("")
{

}

////////////////////////////////////////////////////////////////////////////////
//  GUIPxText virtual destructor                                              //
////////////////////////////////////////////////////////////////////////////////
GUIPxText::~GUIPxText()
{
    m_text = "";
    m_smooth = 0.0f;
    m_color.reset();
}


////////////////////////////////////////////////////////////////////////////////
//  Init pixel text                                                           //
//  return : True if the pixel text is successfully created                   //
////////////////////////////////////////////////////////////////////////////////
bool GUIPxText::init(Texture& texture, float height)
{
    // Check texture handle
    if (!texture.isValid())
    {
        // Invalid texture handle
        return false;
    }

    // Reset pixel text transformations
    resetTransforms();

    // Set pixel text size
    setSize(0.0f, height);

    // Set pixel text texture pointer
    m_texture = &texture;

    // Reset pixel text color
    m_color.set(1.0f, 1.0f, 1.0f, 1.0f);

    // Reset pixel text smooth amount
    m_smooth = 0.1f;

    // Reset pixel text internal string
    m_text = "";

    // Pixel text successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Set pixel text texture                                                    //
//  return : True if pixel text texture is successfully set                   //
////////////////////////////////////////////////////////////////////////////////
bool GUIPxText::setTexture(Texture& texture)
{
    // Check texture handle
    if (!texture.isValid())
    {
        // Invalid texture handle
        return false;
    }

    // Set pixel text texture pointer
    m_texture = &texture;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Set pixel text internal string                                            //
////////////////////////////////////////////////////////////////////////////////
void GUIPxText::setText(const std::string& text)
{
    // Set pixel text internal string
    m_text = text;

    // Update pixel text width
    if (m_text.length() <= 0)
    {
        m_size.vec[0] = 0.0f;
    }
    else
    {
        m_size.vec[0] = (m_size.vec[1] +
            (m_size.vec[1] * PixelTextDefaultXOffset * (m_text.length()-1))
        );
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Set pixel text color                                                      //
////////////////////////////////////////////////////////////////////////////////
void GUIPxText::setColor(const Vector4& color)
{
    m_color.vec[0] = color.vec[0];
    m_color.vec[1] = color.vec[1];
    m_color.vec[2] = color.vec[2];
    m_color.vec[3] = color.vec[3];
}

////////////////////////////////////////////////////////////////////////////////
//  Set pixel text color                                                      //
////////////////////////////////////////////////////////////////////////////////
void GUIPxText::setColor(float red, float green, float blue, float alpha)
{
    m_color.vec[0] = red;
    m_color.vec[1] = green;
    m_color.vec[2] = blue;
    m_color.vec[3] = alpha;
}


////////////////////////////////////////////////////////////////////////////////
//  Render pixel text                                                         //
////////////////////////////////////////////////////////////////////////////////
void GUIPxText::render()
{
    // Compute pixel text transformations
    m_matrix.setIdentity();
    m_matrix.translate(m_position);
    m_matrix.rotateZ(m_angle);
    m_matrix.translate(-m_origin);
    m_matrix.scale(m_size.vec[1], m_size.vec[1]);

    // Move to the first character
    m_matrix.translateX(PixelTextDefaultXStart);

    // Send uniforms constants
    GRenderer.currentShader->sendColor(m_color);
    GRenderer.currentShader->sendSize(
        Vector2(PixelTextDefaultUVWidth, PixelTextDefaultUVHeight)
    );
    GRenderer.currentShader->sendTime(m_smooth*PixelTextDefaultSmoothFactor);

    // Draw pixel text characters
    for (size_t i = 0; i < m_text.length(); ++i)
    {
        // Get char code
        char charCode = m_text[i]-32;
        if (charCode < 0) { charCode = 31; }
        if (charCode > 94) { charCode = 31; }
        int charX = (charCode%16);
        int charY = (charCode/16);
        if (charX <= 0) { charX = 0; }
        if (charX >= 15) { charX = 15; }
        if (charY <= 0) { charY = 0; }
        if (charY >= 5) { charY = 5; }

        // Upload model matrix
        GRenderer.currentShader->sendModelMatrix(m_matrix);

        // Send UV uniforms constants
        GRenderer.currentShader->sendOffset(Vector2(
            (charX*PixelTextDefaultUVWidth), (charY*PixelTextDefaultUVHeight)
        ));

        // Render current character
        GRenderer.currentBuffer->render();

        // Move to the next character
        m_matrix.translateX(PixelTextDefaultXOffset);
    }
}
