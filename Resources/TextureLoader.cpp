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
//     Resources/TextureLoader.cpp : Texture loading management               //
////////////////////////////////////////////////////////////////////////////////
#include "TextureLoader.h"
#include "Resources.h"


////////////////////////////////////////////////////////////////////////////////
//  Texture loaded generic callback function                                  //
////////////////////////////////////////////////////////////////////////////////
inline void OnTextureLoaded(void* arg, void* buffer, int size)
{
    GResources.textures.onTextureLoaded(arg, buffer, size);
}

////////////////////////////////////////////////////////////////////////////////
//  Texture error generic callback function                                   //
////////////////////////////////////////////////////////////////////////////////
inline void OnTextureError(void* arg)
{
    GResources.textures.onTextureError(arg);
}


////////////////////////////////////////////////////////////////////////////////
//  TextureLoader default constructor                                         //
////////////////////////////////////////////////////////////////////////////////
TextureLoader::TextureLoader() :
m_state(TEXTURELOADER_STATE_NONE),
m_stateMutex(),
m_texturesHigh(0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  TextureLoader virtual destructor                                          //
////////////////////////////////////////////////////////////////////////////////
TextureLoader::~TextureLoader()
{

}


////////////////////////////////////////////////////////////////////////////////
//  TextureLoader thread process                                              //
////////////////////////////////////////////////////////////////////////////////
void TextureLoader::process()
{
    TextureLoaderState state = TEXTURELOADER_STATE_NONE;
    m_stateMutex.lock();
    state = m_state;
    m_stateMutex.unlock();

    switch (state)
    {
        case TEXTURELOADER_STATE_NONE:
            // Boot to init state
            m_stateMutex.lock();
            m_state = TEXTURELOADER_STATE_INIT;
            m_stateMutex.unlock();
            break;

        case TEXTURELOADER_STATE_INIT:
            // Init texture loader
            if (init())
            {
                m_stateMutex.lock();
                m_state = TEXTURELOADER_STATE_LOADEMBEDDED;
                m_stateMutex.unlock();
            }
            else
            {
                m_stateMutex.lock();
                m_state = TEXTURELOADER_STATE_ERROR;
                m_stateMutex.unlock();
            }
            break;

        case TEXTURELOADER_STATE_LOADEMBEDDED:
            // Load embedded textures
            if (loadEmbeddedTextures())
            {
                m_stateMutex.lock();
                m_state = TEXTURELOADER_STATE_IDLE;
                m_stateMutex.unlock();
            }
            else
            {
                m_stateMutex.lock();
                m_state = TEXTURELOADER_STATE_ERROR;
                m_stateMutex.unlock();
            }
            break;

        case TEXTURELOADER_STATE_IDLE:
            // Texture loader in idle state
            SysSleep(TextureLoaderIdleSleepTime);
            break;

        case TEXTURELOADER_STATE_PRELOAD:
            // Preload textures assets
            if (preloadTextures())
            {
                m_stateMutex.lock();
                m_state = TEXTURELOADER_STATE_IDLE;
                m_stateMutex.unlock();
            }
            else
            {
                m_stateMutex.lock();
                m_state = TEXTURELOADER_STATE_ERROR;
                m_stateMutex.unlock();
            }
            break;

        case TEXTURELOADER_STATE_LOAD:
            // Load textures assets
            if (loadTextures())
            {
                m_stateMutex.lock();
                m_state = TEXTURELOADER_STATE_IDLE;
                m_stateMutex.unlock();
            }
            else
            {
                m_stateMutex.lock();
                m_state = TEXTURELOADER_STATE_ERROR;
                m_stateMutex.unlock();
            }
            break;

        case TEXTURELOADER_STATE_ERROR:
            // Texture loader error
            SysSleep(TextureLoaderErrorSleepTime);
            break;

        default:
            // Invalid state
            m_stateMutex.lock();
            m_state = TEXTURELOADER_STATE_ERROR;
            m_stateMutex.unlock();
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////
//  Init TextureLoader                                                        //
//  return : True if texture loader is ready                                  //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::init()
{
    // Start renderer job
    GRenderer.startJob();

    // Allocate high textures
    m_texturesHigh = new (std::nothrow) Texture[TEXTURE_ASSETSCOUNT];
    if (!m_texturesHigh)
    {
        // Could not allocate high textures
        return false;
    }

    // Texture loader ready
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Start preloading textures assets                                          //
//  return : True if textures assets are preloading                           //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::startPreload()
{
    bool preLoading = false;
    m_stateMutex.lock();

    // Check texture loader state
    if (m_state == TEXTURELOADER_STATE_IDLE)
    {
        // Switch to preload state
        m_state = TEXTURELOADER_STATE_PRELOAD;
        preLoading = true;
    }

    m_stateMutex.unlock();
    return preLoading;
}

////////////////////////////////////////////////////////////////////////////////
//  Start loading textures assets                                             //
//  return : True if textures assets are loading                              //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::startLoading()
{
    bool loading = false;
    m_stateMutex.lock();

    // Check texture loader state
    if (m_state == TEXTURELOADER_STATE_IDLE)
    {
        // Switch to load state
        m_state = TEXTURELOADER_STATE_LOAD;
        loading = true;
    }

    m_stateMutex.unlock();
    return loading;
}

////////////////////////////////////////////////////////////////////////////////
//  Get texture loader state                                                  //
//  return : Current texture loader state                                     //
////////////////////////////////////////////////////////////////////////////////
TextureLoaderState TextureLoader::getState()
{
    TextureLoaderState state = TEXTURELOADER_STATE_NONE;
    m_stateMutex.lock();
    state = m_state;
    m_stateMutex.unlock();
    return state;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy texture loader                                                    //
////////////////////////////////////////////////////////////////////////////////
void TextureLoader::destroyTextureLoader()
{
    // Destroy high textures
    for (int i = 0; i < TEXTURE_ASSETSCOUNT; ++i)
    {
        m_texturesHigh[i].destroyTexture();
    }
    if (m_texturesHigh) { delete[] m_texturesHigh; }
    m_texturesHigh = 0;
}


////////////////////////////////////////////////////////////////////////////////
//  Upload texture to graphics memory                                         //
//  return : True if texture is successfully uploaded                         //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::uploadTexture(unsigned int& handle,
    uint32_t width, uint32_t height, uint32_t mipLevels,
    const unsigned char* data, bool smooth, TextureRepeatMode repeat)
{
    // Set current thread as current context
    GSysWindow.setThread();

    // Create texture
    glGenTextures(1, &handle);
    if (!handle)
    {
        // Unable to create texture
        SysMessage::box() << "[0x300D] Unable to create texture\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // Upload texture data
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, width, height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, data
    );
    if (repeat == TEXTUREMODE_REPEAT)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    if (smooth)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Generate texture mipmaps
    if (!generateTextureMipmaps(handle, width, height, mipLevels))
    {
        // Could not generate texture mipmaps
        return false;
    }

    // Texture successfully uploaded
    GSysWindow.releaseThread();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Generate texture mipmaps                                                  //
//  return : True if texture mipmaps are generated                            //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::generateTextureMipmaps(unsigned int& handle,
    uint32_t width, uint32_t height, uint32_t mipLevels)
{
    // Check mip levels
    if (mipLevels <= 1)
    {
        // No mipmaps generation
        return true;
    }

    // Texture mipmaps generated
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Texture loaded callback function                                          //
////////////////////////////////////////////////////////////////////////////////
void TextureLoader::onTextureLoaded(void* arg, void* buffer, int size)
{
    // Load texture
    PNGFile pngfile;
    if (!pngfile.loadImage((unsigned char*)buffer, size))
    {
        return;
    }
    if (!m_texturesHigh[TEXTURE_TEST].createTexture(
        pngfile.getWidth(), pngfile.getHeight(), pngfile.getImage(),
        false, false, TEXTUREMODE_CLAMP))
    {
        // Could not load texture
        return;
    }
    pngfile.destroyImage();
}

////////////////////////////////////////////////////////////////////////////////
//  Texture error callback function                                           //
////////////////////////////////////////////////////////////////////////////////
void TextureLoader::onTextureError(void* arg)
{

}


////////////////////////////////////////////////////////////////////////////////
//  Load embedded textures                                                    //
//  return : True if embedded textures are successfully loaded                //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::loadEmbeddedTextures()
{
    // Embedded textures successfully loaded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Preload textures assets                                                   //
//  return : True if textures assets are preloaded                            //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::preloadTextures()
{
    // Load test texture
    emscripten_async_wget_data(
        "textures/testsprite.png", (void*)TEXTURE_TEST,
        OnTextureLoaded, OnTextureError
    );

    // Textures assets are successfully preloaded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Load textures assets                                                      //
//  return : True if textures assets are loaded                               //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::loadTextures()
{
    // Textures assets are successfully loaded
    return true;
}
