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
//     Renderer/Shapes/EllipseShape.h : Ellipse shape management              //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_SHAPES_ELLIPSESHAPE_HEADER
#define WOS_RENDERER_SHAPES_ELLIPSESHAPE_HEADER

    #include "../../System/System.h"
    #include "../../Math/Math.h"
    #include "../../Math/Vector4.h"
    #include "../../Math/Matrix4x4.h"
    #include "../../Math/Transform2.h"


    ////////////////////////////////////////////////////////////////////////////
    //  EllipseShape class definition                                         //
    ////////////////////////////////////////////////////////////////////////////
    class EllipseShape : public Transform2
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  EllipseShape default constructor                              //
            ////////////////////////////////////////////////////////////////////
            EllipseShape();

            ////////////////////////////////////////////////////////////////////
            //  EllipseShape virtual destructor                               //
            ////////////////////////////////////////////////////////////////////
            virtual ~EllipseShape();


            ////////////////////////////////////////////////////////////////////
            //  Init ellipse                                                  //
            //  return : True if the ellipse is successfully created          //
            ////////////////////////////////////////////////////////////////////
            bool init(float width, float height);

            ////////////////////////////////////////////////////////////////////
            //  Set ellipse color                                             //
            ////////////////////////////////////////////////////////////////////
            void setColor(const Vector4& color);

            ////////////////////////////////////////////////////////////////////
            //  Set ellipse color                                             //
            ////////////////////////////////////////////////////////////////////
            void setColor(float red, float green, float blue, float alpha);

            ////////////////////////////////////////////////////////////////////
            //  Set ellipse red channel                                       //
            ////////////////////////////////////////////////////////////////////
            inline void setRed(float red)
            {
                m_color.vec[0] = red;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set ellipse green channel                                     //
            ////////////////////////////////////////////////////////////////////
            inline void setGreen(float green)
            {
                m_color.vec[1] = green;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set ellipse blue channel                                      //
            ////////////////////////////////////////////////////////////////////
            inline void setBlue(float blue)
            {
                m_color.vec[2] = blue;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set ellipse alpha channel                                     //
            ////////////////////////////////////////////////////////////////////
            inline void setAlpha(float alpha)
            {
                m_color.vec[3] = alpha;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set ellipse smooth amount                                     //
            ////////////////////////////////////////////////////////////////////
            inline void setSmooth(float smooth)
            {
                // Clamp smooth amount
                if (smooth <= 0.0f) { smooth = 0.0f; }
                if (smooth >= 1.0f) { smooth = 1.0f; }

                // Set smooth amount
                m_smooth = smooth;
            }


            ////////////////////////////////////////////////////////////////////
            //  Render ellipse                                                //
            ////////////////////////////////////////////////////////////////////
            void render();


        private:
            ////////////////////////////////////////////////////////////////////
            //  EllipseShape private copy constructor : Not copyable          //
            ////////////////////////////////////////////////////////////////////
            EllipseShape(const EllipseShape&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  EllipseShape private copy operator : Not copyable             //
            ////////////////////////////////////////////////////////////////////
            EllipseShape& operator=(const EllipseShape&) = delete;


        private:
            Vector4             m_color;            // Ellipse color
            float               m_smooth;           // Ellipse smooth amount
    };


#endif // WOS_RENDERER_SHAPES_ELLIPSESHAPE_HEADER
