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
//     Renderer/Texture.cpp : Texture management                              //
////////////////////////////////////////////////////////////////////////////////
#include "Texture.h"
#include "../Resources/Resources.h"


////////////////////////////////////////////////////////////////////////////////
//  Texture default constructor                                               //
////////////////////////////////////////////////////////////////////////////////
Texture::Texture() :
m_handle(0),
m_width(0),
m_height(0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  Texture destructor                                                        //
////////////////////////////////////////////////////////////////////////////////
Texture::~Texture()
{
    destroyTexture();
}


////////////////////////////////////////////////////////////////////////////////
//  Create texture                                                            //
//  return : True if texture is successfully created                          //
////////////////////////////////////////////////////////////////////////////////
bool Texture::createTexture(uint32_t width, uint32_t height,
    const unsigned char* data,
    bool mipmaps, bool smooth, TextureRepeatMode repeat)
{
    // Check texture handle
    if (m_handle)
    {
        // Destroy current texture
        destroyTexture();
    }

    // Check texture size
    if ((width <= 0) || (width > TextureMaxWidth) ||
        (height <= 0) || (height > TextureMaxHeight))
    {
        // Invalid texture size
        SysMessage::box() << "[0x300C] Invalid texture size :\n";
        SysMessage::box() << width << "x" << height << "px";
        return false;
    }

    // Check texture data
    if (!data)
    {
        // Invalid texture data
        return false;
    }

    // Set mip levels
    uint32_t mipLevels = 1;
    if (mipmaps)
    {
        mipLevels = (Math::log2(((width > height) ? width : height)) + 1);
    }
    if (mipLevels <= 1) { mipLevels = 1; }

    // Upload texture to graphics memory
    if (!GResources.textures.uploadTexture(
        m_handle, width, height, mipLevels, data, smooth, repeat))
    {
        // Could not upload texture to graphics memory
        return false;
    }

    // Set texture size
    m_width = width;
    m_height = height;

    // Texture successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy texture                                                           //
////////////////////////////////////////////////////////////////////////////////
void Texture::destroyTexture()
{
    if (m_handle)
    {
        // Destroy texture
        GSysWindow.setThread();
        glDeleteTextures(1, &m_handle);
        GSysWindow.releaseThread();
    }
    m_handle = 0;
    m_height = 0;
    m_width = 0;
}
