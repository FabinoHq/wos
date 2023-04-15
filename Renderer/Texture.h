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
//     Renderer/Texture.h : Texture management                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_TEXTURE_HEADER
#define WOS_RENDERER_TEXTURE_HEADER

    #include "../System/System.h"
    #include "../Math/Math.h"

    #include <cstdint>


    ////////////////////////////////////////////////////////////////////////////
    //  Texture settings                                                      //
    ////////////////////////////////////////////////////////////////////////////
    const uint32_t TextureMaxWidth = 4096;
    const uint32_t TextureMaxHeight = 4096;


    ////////////////////////////////////////////////////////////////////////////
    //  TextureRepeatMode enumeration                                         //
    ////////////////////////////////////////////////////////////////////////////
    enum TextureRepeatMode
    {
        TEXTUREMODE_CLAMP = 0,
        TEXTUREMODE_REPEAT = 1,
        TEXTUREMODE_MIRROR = 2
    };


    ////////////////////////////////////////////////////////////////////////////
    //  Texture class definition                                              //
    ////////////////////////////////////////////////////////////////////////////
    class Texture
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Texture default constructor                                   //
            ////////////////////////////////////////////////////////////////////
            Texture();

            ////////////////////////////////////////////////////////////////////
            //  Texture destructor                                            //
            ////////////////////////////////////////////////////////////////////
            ~Texture();


            ////////////////////////////////////////////////////////////////////
            //  Create texture                                                //
            //  return : True if texture is successfully created              //
            ////////////////////////////////////////////////////////////////////
            bool createTexture(uint32_t width, uint32_t height,
                const unsigned char* data,
                bool mipmaps = false, bool smooth = true,
                TextureRepeatMode repeat = TEXTUREMODE_CLAMP);

            ////////////////////////////////////////////////////////////////////
            //  Bind texture                                                  //
            ////////////////////////////////////////////////////////////////////
            inline void bind()
            {

            }

            ////////////////////////////////////////////////////////////////////
            //  Destroy texture                                               //
            ////////////////////////////////////////////////////////////////////
            void destroyTexture();


            ////////////////////////////////////////////////////////////////////
            //  Check if the texture has a valid handle                       //
            //  return : True if the texture is valid                         //
            ////////////////////////////////////////////////////////////////////
            inline bool isValid()
            {
                return false;
            }


        private:
            ////////////////////////////////////////////////////////////////////
            //  Texture private copy constructor : Not copyable               //
            ////////////////////////////////////////////////////////////////////
            Texture(const Texture&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Texture private copy operator : Not copyable                  //
            ////////////////////////////////////////////////////////////////////
            Texture& operator=(const Texture&) = delete;


        private:
            uint32_t            m_width;            // Texture width
            uint32_t            m_height;           // Texture height
    };


#endif // WOS_RENDERER_TEXTURE_HEADER
