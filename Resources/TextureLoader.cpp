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


////////////////////////////////////////////////////////////////////////////////
//  Texture loaded callback function                                          //
////////////////////////////////////////////////////////////////////////////////
void OnTextureLoaded(void* arg, void* buffer, int size)
{
    // Get callback data
    TextureCallbackData* callbackData = (TextureCallbackData*)arg;
    if (!callbackData) { return; }

    // Allocate data memory for copy
    callbackData->mutex.lock();
    callbackData->data = new (std::nothrow) unsigned char[size];
    if (!callbackData->data)
    {
        // Could not allocate data memory
        callbackData->data = 0;
        callbackData->size = 0;
        callbackData->state = TEXTURELOADER_CALLBACK_ERROR;
        callbackData->mutex.unlock();
        return;
    }

    // Copy downloaded data into memory
    memcpy(callbackData->data, buffer, size);
    callbackData->size = size;
    callbackData->state = TEXTURELOADER_CALLBACK_LOADED;
    callbackData->mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
//  Texture error callback function                                           //
////////////////////////////////////////////////////////////////////////////////
void OnTextureError(void* arg)
{
    // Get callback data
    TextureCallbackData* callbackData = (TextureCallbackData*)arg;
    if (!callbackData) { return; }

    // Set callback data error
    callbackData->mutex.lock();
    callbackData->data = 0;
    callbackData->size = 0;
    callbackData->state = TEXTURELOADER_CALLBACK_ERROR;
    callbackData->mutex.unlock();
}


////////////////////////////////////////////////////////////////////////////////
//  TextureLoader default constructor                                         //
////////////////////////////////////////////////////////////////////////////////
TextureLoader::TextureLoader() :
m_state(TEXTURELOADER_STATE_NONE),
m_stateMutex(),
m_texturesGUI(0),
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
    // Allocate GUI textures
    m_texturesGUI = new (std::nothrow) Texture[TEXTURE_GUICOUNT];
    if (!m_texturesGUI)
    {
        // Could not allocate GUI textures
        return false;
    }

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

    // Destroy GUI textures
    for (int i = 0; i < TEXTURE_GUICOUNT; ++i)
    {
        m_texturesGUI[i].destroyTexture();
    }
    if (m_texturesGUI) { delete[] m_texturesGUI; }
    m_texturesGUI = 0;
}


////////////////////////////////////////////////////////////////////////////////
//  Load texture asynchronously and wait for callback                         //
//  return : True if texture is loaded, false otherwise                       //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::loadTextureAsync(Texture& texture, const char* path,
    bool mipmaps, bool smooth, TextureRepeatMode repeat)
{
    // Reset callback data
    TextureCallbackData callbackData;
    callbackData.mutex.lock();
    callbackData.state = TEXTURELOADER_CALLBACK_NONE;
    callbackData.data = 0;
    callbackData.size = 0;
    callbackData.mutex.unlock();

    // Download texture asynchronously
    emscripten_async_wget_data(
        path, (void*)&callbackData, OnTextureLoaded, OnTextureError
    );
    TextureCallbackState state = TEXTURELOADER_CALLBACK_NONE;

    // Wait for the texture to be downloaded
    while (state == TEXTURELOADER_CALLBACK_NONE)
    {
        callbackData.mutex.lock();
        state = callbackData.state;
        callbackData.mutex.unlock();
        SysSleep(TextureLoaderWaitAsyncSleepTime);
    }
    if ((state == TEXTURELOADER_CALLBACK_ERROR) || (!callbackData.data))
    {
        // Could not download texture
        return false;
    }

    // Load texture from PNG buffer
    PNGFile pngfile;
    if (!pngfile.loadImage(callbackData.data, callbackData.size))
    {
        // Could not load PNG buffer
        if (callbackData.data) { delete[] callbackData.data; }
        return false;
    }
    if (!texture.createTexture(
        pngfile.getWidth(), pngfile.getHeight(), pngfile.getImage(),
        mipmaps, smooth, repeat))
    {
        // Could not create texture
        if (callbackData.data) { delete[] callbackData.data; }
        return false;
    }
    pngfile.destroyImage();
    if (callbackData.data) { delete[] callbackData.data; }

    // Texture is successfully loaded
    return true;
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

    // Repeat mode
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

    // Smooth mode (linear interpolation)
    if (smooth)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Anisotropic filtering
        switch (GSysSettings.getAnisotropicFilteringMode())
        {
            case ANISOTROPIC_FILTERING_2X:
                glTexParameterf(
                    GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f
                );
                break;
            case ANISOTROPIC_FILTERING_4X:
                glTexParameterf(
                    GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f
                );
                break;
            case ANISOTROPIC_FILTERING_8X:
                glTexParameterf(
                    GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f
                );
                break;
            case ANISOTROPIC_FILTERING_16X:
                glTexParameterf(
                    GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f
                );
                break;
            default:
                break;
        }
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
    // Check texture size
    if ((width <= 0) || (width > TextureMaxWidth) ||
        (height <= 0) || (height > TextureMaxHeight))
    {
        // Invalid texture size
        return false;
    }

    // Check mip levels
    if (mipLevels <= 1)
    {
        // No mipmaps generation
        return true;
    }

    // Generate texture mipmaps
    glBindTexture(GL_TEXTURE_2D, handle);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Texture mipmaps generated
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Load embedded textures                                                    //
//  return : True if embedded textures are successfully loaded                //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::loadEmbeddedTextures()
{
    // Load window texture
    if (!loadTextureAsync(m_texturesGUI[TEXTURE_WINDOW],
        "textures/window.png",
        false, true, TEXTUREMODE_CLAMP))
    {
        // Could not load pixel font texture
        return false;
    }

    // Load pixel font texture
    if (!loadTextureAsync(m_texturesGUI[TEXTURE_PIXELFONT],
        "fonts/wospxfont.png",
        false, true, TEXTUREMODE_CLAMP))
    {
        // Could not load pixel font texture
        return false;
    }

    // Embedded textures are successfully loaded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Preload textures assets                                                   //
//  return : True if textures assets are preloaded                            //
////////////////////////////////////////////////////////////////////////////////
bool TextureLoader::preloadTextures()
{
    // Load test texture
    if (!loadTextureAsync(m_texturesHigh[TEXTURE_TEST],
        "textures/testsprite.png",
        false, false, TEXTUREMODE_CLAMP))
    {
        // Could not load test texture
        return false;
    }

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
