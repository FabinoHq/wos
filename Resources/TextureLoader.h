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
//     Resources/TextureLoader.h : Texture loading management                 //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RESOURCES_TEXTURELOADER_HEADER
#define WOS_RESOURCES_TEXTURELOADER_HEADER

    #include <emscripten/html5.h>
    #include <GLES2/gl2.h>

    #include "../System/System.h"
    #include "../System/SysThread.h"
    #include "../System/SysMutex.h"

    #include "../Renderer/Renderer.h"
    #include "../Renderer/Texture.h"

    #include "../Images/PNGFile.h"

    #include <cstdint>
    #include <new>


    ////////////////////////////////////////////////////////////////////////////
    //  TextureLoader settings                                                //
    ////////////////////////////////////////////////////////////////////////////
    const uint32_t TextureMaxWidth = 4096;
    const uint32_t TextureMaxHeight = 4096;
    const uint32_t TextureMaxLayers = 8;
    const uint32_t CubeMapMaxWidth = 2048;
    const uint32_t CubeMapMaxHeight = 2048;
    const uint32_t TextureMaxSize = (TextureMaxWidth*TextureMaxHeight*4);
    const uint32_t TextureArrayMaxSize =
        (TextureMaxWidth*TextureMaxHeight*TextureMaxLayers*4);
    const uint32_t CubeMapMaxSize = (CubeMapMaxWidth*CubeMapMaxHeight*4*6);
    const double TextureLoaderIdleSleepTime = 0.01;
    const double TextureLoaderErrorSleepTime = 0.1;


    ////////////////////////////////////////////////////////////////////////////
    //  TexturesGUI enumeration                                               //
    ////////////////////////////////////////////////////////////////////////////
    enum TexturesGUI
    {
        TEXTURE_GUICOUNT = 0
    };

    ////////////////////////////////////////////////////////////////////////////
    //  TexturesAssets enumeration                                            //
    ////////////////////////////////////////////////////////////////////////////
    enum TexturesAssets
    {
        TEXTURE_TEST = 0,

        TEXTURE_ASSETSCOUNT = 1
    };

    ////////////////////////////////////////////////////////////////////////////
    //  TexturesArrays enumeration                                            //
    ////////////////////////////////////////////////////////////////////////////
    enum TexturesArrays
    {
        TEXTURE_ARRAYSCOUNT = 0
    };

    ////////////////////////////////////////////////////////////////////////////
    //  TexturesCubeMaps enumeration                                          //
    ////////////////////////////////////////////////////////////////////////////
    enum TexturesCubeMaps
    {
        TEXTURE_CUBEMAPCOUNT = 0
    };


    ////////////////////////////////////////////////////////////////////////////
    //  TextureLoaderState enumeration                                        //
    ////////////////////////////////////////////////////////////////////////////
    enum TextureLoaderState
    {
        TEXTURELOADER_STATE_NONE = 0,
        TEXTURELOADER_STATE_INIT = 1,
        TEXTURELOADER_STATE_LOADEMBEDDED = 2,

        TEXTURELOADER_STATE_IDLE = 3,
        TEXTURELOADER_STATE_PRELOAD = 4,
        TEXTURELOADER_STATE_LOAD = 5,

        TEXTURELOADER_STATE_ERROR = 6
    };


    ////////////////////////////////////////////////////////////////////////////
    //  TextureLoader class definition                                        //
    ////////////////////////////////////////////////////////////////////////////
    class TextureLoader : public SysThread
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  TextureLoader default constructor                             //
            ////////////////////////////////////////////////////////////////////
            TextureLoader();

            ////////////////////////////////////////////////////////////////////
            //  TextureLoader virtual destructor                              //
            ////////////////////////////////////////////////////////////////////
            virtual ~TextureLoader();


            ////////////////////////////////////////////////////////////////////
            //  TextureLoader thread process                                  //
            ////////////////////////////////////////////////////////////////////
            virtual void process();


            ////////////////////////////////////////////////////////////////////
            //  Init TextureLoader                                            //
            //  return : True if texture loader is ready                      //
            ////////////////////////////////////////////////////////////////////
            bool init();

            ////////////////////////////////////////////////////////////////////
            //  Start preloading textures assets                              //
            //  return : True if textures assets are preloading               //
            ////////////////////////////////////////////////////////////////////
            bool startPreload();

            ////////////////////////////////////////////////////////////////////
            //  Start loading textures assets                                 //
            //  return : True if textures assets are loading                  //
            ////////////////////////////////////////////////////////////////////
            bool startLoading();

            ////////////////////////////////////////////////////////////////////
            //  Get texture loader state                                      //
            //  return : Current texture loader state                         //
            ////////////////////////////////////////////////////////////////////
            TextureLoaderState getState();

            ////////////////////////////////////////////////////////////////////
            //  Get high texture                                              //
            //  return : high texture                                         //
            ////////////////////////////////////////////////////////////////////
            inline Texture& high(TexturesAssets texture)
            {
                return m_texturesHigh[texture];
            }

            ////////////////////////////////////////////////////////////////////
            //  Destroy texture loader                                        //
            ////////////////////////////////////////////////////////////////////
            void destroyTextureLoader();


            ////////////////////////////////////////////////////////////////////
            //  Upload texture to graphics memory                             //
            //  return : True if texture is successfully uploaded             //
            ////////////////////////////////////////////////////////////////////
            bool uploadTexture(unsigned int& handle,
                uint32_t width, uint32_t height, uint32_t mipLevels,
                const unsigned char* data,
                bool smooth, TextureRepeatMode repeat);

            ////////////////////////////////////////////////////////////////////
            //  Generate texture mipmaps                                      //
            //  return : True if texture mipmaps are generated                //
            ////////////////////////////////////////////////////////////////////
            bool generateTextureMipmaps(unsigned int& handle,
                uint32_t width, uint32_t height, uint32_t mipLevels);


            ////////////////////////////////////////////////////////////////////
            //  Texture loaded callback function                              //
            ////////////////////////////////////////////////////////////////////
            void onTextureLoaded(void* arg, void* buffer, int size);

            ////////////////////////////////////////////////////////////////////
            //  Texture error callback function                               //
            ////////////////////////////////////////////////////////////////////
            void onTextureError(void* arg);


        private:
            ////////////////////////////////////////////////////////////////////
            //  Load embedded textures                                        //
            //  return : True if embedded textures are successfully loaded    //
            ////////////////////////////////////////////////////////////////////
            bool loadEmbeddedTextures();

            ////////////////////////////////////////////////////////////////////
            //  Preload textures assets                                       //
            //  return : True if textures assets are preloaded                //
            ////////////////////////////////////////////////////////////////////
            bool preloadTextures();

            ////////////////////////////////////////////////////////////////////
            //  Load textures assets                                          //
            //  return : True if textures assets are loaded                   //
            ////////////////////////////////////////////////////////////////////
            bool loadTextures();


        private:
            ////////////////////////////////////////////////////////////////////
            //  TextureLoader private copy constructor : Not copyable         //
            ////////////////////////////////////////////////////////////////////
            TextureLoader(const TextureLoader&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  TextureLoader private copy operator : Not copyable            //
            ////////////////////////////////////////////////////////////////////
            TextureLoader& operator=(const TextureLoader&) = delete;


        private:
            TextureLoaderState      m_state;            // TextureLoader state
            SysMutex                m_stateMutex;       // State mutex

            Texture*                m_texturesHigh;     // High textures
    };


#endif // WOS_RESOURCES_TEXTURELOADER_HEADER
