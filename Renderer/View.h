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
//     Renderer/View.h : View management                                      //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_VIEW_HEADER
#define WOS_RENDERER_VIEW_HEADER

    #include "../System/System.h"
    #include "../Math/Math.h"
    #include "../Math/Vector2.h"
    #include "../Math/Matrix4x4.h"
    #include "../Math/Transform2.h"

    #include "Shader.h"

    #include <cstdint>
    #include <cstring>


    ////////////////////////////////////////////////////////////////////////////
    //  View class definition                                                 //
    ////////////////////////////////////////////////////////////////////////////
    class View : public Transform2
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  View default constructor                                      //
            ////////////////////////////////////////////////////////////////////
            View();

            ////////////////////////////////////////////////////////////////////
            //  View virtual destructor                                       //
            ////////////////////////////////////////////////////////////////////
            virtual ~View();


            ////////////////////////////////////////////////////////////////////
            //  Init view                                                     //
            //  return : True if the view is successfully created             //
            ////////////////////////////////////////////////////////////////////
            bool init();

            ////////////////////////////////////////////////////////////////////
            //  Destroy view                                                  //
            ////////////////////////////////////////////////////////////////////
            void destroyView();

            ////////////////////////////////////////////////////////////////////
            //  Compute view                                                  //
            //  return : True if the view is successfully computed            //
            ////////////////////////////////////////////////////////////////////
            bool compute(float ratio);

            ////////////////////////////////////////////////////////////////////
            //  Bind view                                                     //
            ////////////////////////////////////////////////////////////////////
            inline void bind(Shader& shader)
            {
                // Upload matrices
                shader.setProjectionMatrix(m_projMatrix);
                shader.setProjViewMatrix(m_projViewMatrix);
            }


        private:
            ////////////////////////////////////////////////////////////////////
            //  View private copy constructor : Not copyable                  //
            ////////////////////////////////////////////////////////////////////
            View(const View&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  View private copy operator : Not copyable                     //
            ////////////////////////////////////////////////////////////////////
            View& operator=(const View&) = delete;


        private:
            Matrix4x4           m_projMatrix;       // Projection matrix
            Matrix4x4           m_projViewMatrix;   // Projview matrix
    };


#endif // WOS_RENDERER_VIEW_HEADER
