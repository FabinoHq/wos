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
//     Renderer/Sprite.h : Sprite management                                  //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_SPRITE_HEADER
#define WOS_RENDERER_SPRITE_HEADER

    #include "../System/System.h"
    #include "../Math/Math.h"
    #include "../Math/Vector2.h"
    #include "../Math/Vector4.h"
    #include "../Math/Matrix4x4.h"
    #include "../Math/Transform2.h"

    #include "Renderer.h"
    #include "Texture.h"


    ////////////////////////////////////////////////////////////////////////////
    //  Sprite class definition                                               //
    ////////////////////////////////////////////////////////////////////////////
    class Sprite : public Transform2
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Sprite default constructor                                    //
            ////////////////////////////////////////////////////////////////////
            Sprite();

            ////////////////////////////////////////////////////////////////////
            //  Sprite virtual destructor                                     //
            ////////////////////////////////////////////////////////////////////
            virtual ~Sprite();


            ////////////////////////////////////////////////////////////////////
            //  Init sprite                                                   //
            //  return : True if the sprite is successfully created           //
            ////////////////////////////////////////////////////////////////////
            bool init(Texture& texture, float width, float height);

            ////////////////////////////////////////////////////////////////////
            //  Set sprite texture                                            //
            //  return : True if sprite texture is successfully set           //
            ////////////////////////////////////////////////////////////////////
            bool setTexture(Texture& texture);

            ////////////////////////////////////////////////////////////////////
            //  Set sprite color                                              //
            ////////////////////////////////////////////////////////////////////
            void setColor(const Vector4& color);

            ////////////////////////////////////////////////////////////////////
            //  Set sprite color                                              //
            ////////////////////////////////////////////////////////////////////
            void setColor(float red, float green, float blue, float alpha);

            ////////////////////////////////////////////////////////////////////
            //  Set sprite red channel                                        //
            ////////////////////////////////////////////////////////////////////
            inline void setRed(float red)
            {
                m_color.vec[0] = red;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite green channel                                      //
            ////////////////////////////////////////////////////////////////////
            inline void setGreen(float green)
            {
                m_color.vec[1] = green;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite blue channel                                       //
            ////////////////////////////////////////////////////////////////////
            inline void setBlue(float blue)
            {
                m_color.vec[2] = blue;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite alpha channel                                      //
            ////////////////////////////////////////////////////////////////////
            inline void setAlpha(float alpha)
            {
                m_color.vec[3] = alpha;
            }


            ////////////////////////////////////////////////////////////////////
            //  Set sprite subrectangle                                       //
            ////////////////////////////////////////////////////////////////////
            void setSubrect(const Vector2& offset, const Vector2& size);

            ////////////////////////////////////////////////////////////////////
            //  Set sprite subrectangle                                       //
            ////////////////////////////////////////////////////////////////////
            void setSubrect(
                float uOffset, float vOffset, float uSize, float vSize);

            ////////////////////////////////////////////////////////////////////
            //  Set sprite UV offset                                          //
            ////////////////////////////////////////////////////////////////////
            inline void setUVOffset(const Vector2& offset)
            {
                m_uvOffset.vec[0] = offset.vec[0];
                m_uvOffset.vec[1] = offset.vec[1];
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite UV offset                                          //
            ////////////////////////////////////////////////////////////////////
            inline void setUVOffset(float uOffset, float vOffset)
            {
                m_uvOffset.vec[0] = uOffset;
                m_uvOffset.vec[1] = vOffset;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite U offset                                           //
            ////////////////////////////////////////////////////////////////////
            inline void setUOffset(float uOffset)
            {
                m_uvOffset.vec[0] = uOffset;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite V offset                                           //
            ////////////////////////////////////////////////////////////////////
            inline void setVOffset(float vOffset)
            {
                m_uvOffset.vec[1] = vOffset;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite UV size                                            //
            ////////////////////////////////////////////////////////////////////
            inline void setUVSize(const Vector2& size)
            {
                m_uvSize.vec[0] = size.vec[0];
                m_uvSize.vec[1] = size.vec[1];
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite UV size                                            //
            ////////////////////////////////////////////////////////////////////
            inline void setUVSize(float uSize, float vSize)
            {
                m_uvSize.vec[0] = uSize;
                m_uvSize.vec[1] = vSize;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite U size                                             //
            ////////////////////////////////////////////////////////////////////
            inline void setUSize(float uSize)
            {
                m_uvSize.vec[0] = uSize;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set sprite V size                                             //
            ////////////////////////////////////////////////////////////////////
            inline void setVSize(float vSize)
            {
                m_uvSize.vec[1] = vSize;
            }


            ////////////////////////////////////////////////////////////////////
            //  Bind sprite texture                                           //
            ////////////////////////////////////////////////////////////////////
            inline void bindTexture()
            {
                m_texture->bind();
            }

            ////////////////////////////////////////////////////////////////////
            //  Render sprite                                                 //
            ////////////////////////////////////////////////////////////////////
            void render();


        private:
            ////////////////////////////////////////////////////////////////////
            //  Sprite private copy constructor : Not copyable                //
            ////////////////////////////////////////////////////////////////////
            Sprite(const Sprite&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Sprite private copy operator : Not copyable                   //
            ////////////////////////////////////////////////////////////////////
            Sprite& operator=(const Sprite&) = delete;


        private:
            Texture*            m_texture;          // Sprite texture pointer
            Vector4             m_color;            // Sprite color
            Vector2             m_uvOffset;         // Sprite UV offset
            Vector2             m_uvSize;           // Sprite UV size
    };


#endif // WOS_RENDERER_SPRITE_HEADER
