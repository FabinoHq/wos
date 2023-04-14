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
//     Renderer/Shader.h : Shader management                                  //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_SHADER_HEADER
#define WOS_RENDERER_SHADER_HEADER

    #include <GLES2/gl2.h>

    #include "../System/System.h"
    #include "../System/SysMessage.h"
    #include "../Math/Matrix4x4.h"


    ////////////////////////////////////////////////////////////////////////////
    //  Renderer class declaration                                            //
    ////////////////////////////////////////////////////////////////////////////
    class Renderer;


    ////////////////////////////////////////////////////////////////////////////
    //  Shader class definition                                               //
    ////////////////////////////////////////////////////////////////////////////
    class Shader
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Shader default constructor                                    //
            ////////////////////////////////////////////////////////////////////
            Shader();

            ////////////////////////////////////////////////////////////////////
            //  Shader destructor                                             //
            ////////////////////////////////////////////////////////////////////
            ~Shader();


            ////////////////////////////////////////////////////////////////////
            //  Create shader                                                 //
            ////////////////////////////////////////////////////////////////////
            bool createShader(const char* vertexShaderSrc,
                const char* fragmentShaderSrc);

            ////////////////////////////////////////////////////////////////////
            //  Get shader uniform location                                   //
            ////////////////////////////////////////////////////////////////////
            inline unsigned int getUniformLocation(const char* uniform)
            {
                return glGetUniformLocation(m_shader, uniform);
            }

            ////////////////////////////////////////////////////////////////////
            //  Set shader uniform value                                      //
            ////////////////////////////////////////////////////////////////////
            inline void setUniformValue(unsigned int location, int value)
            {
                glUniform1iv(location, 1, &value);
            }

            ////////////////////////////////////////////////////////////////////
            //  Set shader uniform vec 2                                      //
            ////////////////////////////////////////////////////////////////////
            inline void setUniformVec2(unsigned int location, Vector2& vec)
            {
                glUniform2fv(location, 1, vec.vec);
            }

            ////////////////////////////////////////////////////////////////////
            //  Set projection matrix                                         //
            ////////////////////////////////////////////////////////////////////
            inline void setProjectionMatrix(Matrix4x4& projMatrix)
            {
                glUniformMatrix4fv(
                    m_projMatrixLoc, 1, GL_FALSE, projMatrix.mat
                );
            }

            ////////////////////////////////////////////////////////////////////
            //  Set view matrix                                               //
            ////////////////////////////////////////////////////////////////////
            inline void setProjViewMatrix(Matrix4x4& projViewMatrix)
            {
                glUniformMatrix4fv(
                    m_projViewMatrixLoc, 1, GL_FALSE, projViewMatrix.mat
                );
            }

            ////////////////////////////////////////////////////////////////////
            //  Set model matrix                                              //
            ////////////////////////////////////////////////////////////////////
            inline void setModelMatrix(Matrix4x4& modelMatrix)
            {
                glUniformMatrix4fv(
                    m_modelMatrixLoc, 1, GL_FALSE, modelMatrix.mat
                );
            }


            ////////////////////////////////////////////////////////////////////
            //  Bind shader                                                   //
            ////////////////////////////////////////////////////////////////////
            void bindShader();


        private:
            ////////////////////////////////////////////////////////////////////
            //  Shader private copy constructor : Not copyable                //
            ////////////////////////////////////////////////////////////////////
            Shader(const Shader&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Shader private copy operator : Not copyable                   //
            ////////////////////////////////////////////////////////////////////
            Shader& operator=(const Shader&) = delete;


        private:
            unsigned int    m_shader;           // Shader handle
            unsigned int    m_projMatrixLoc;    // Projection matrix location
            unsigned int    m_projViewMatrixLoc;// ProjView matrix location
            unsigned int    m_modelMatrixLoc;   // Model matrix location
    };


#endif // WOS_RENDERER_SHADER_HEADER
