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
    #include "../Math/Math.h"
    #include "../Math/Vector2.h"
    #include "../Math/Vector3.h"
    #include "../Math/Vector4.h"
    #include "../Math/Matrix4x4.h"

    #include <cstdint>


    ////////////////////////////////////////////////////////////////////////////
    //  Renderer shader enumeration                                           //
    ////////////////////////////////////////////////////////////////////////////
    enum RendererShader
    {
        RENDERER_SHADER_DEFAULT = 0,
        RENDERER_SHADER_RECTANGLE = 1,
        RENDERER_SHADER_ELLIPSE = 2,
        RENDERER_SHADER_SHAPE = 3,
        RENDERER_SHADER_STATICMESH = 4,

        RENDERER_SHADER_SHADERSCOUNT = 5
    };


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
            //  Destroy shader                                                //
            ////////////////////////////////////////////////////////////////////
            void destroyShader();


            ////////////////////////////////////////////////////////////////////
            //  Get shader vertices location                                  //
            ////////////////////////////////////////////////////////////////////
            inline int32_t getVerticesLocation()
            {
                return m_verticesLoc;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get shader texcoords location                                 //
            ////////////////////////////////////////////////////////////////////
            inline int32_t getTexCoordsLocation()
            {
                return m_texCoordsLoc;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get shader normals location                                   //
            ////////////////////////////////////////////////////////////////////
            inline int32_t getNormalsLocation()
            {
                return m_normalsLoc;
            }

            ////////////////////////////////////////////////////////////////////
            //  Send projview matrix                                          //
            ////////////////////////////////////////////////////////////////////
            inline void sendProjViewMatrix(Matrix4x4& projViewMatrix)
            {
                glUniformMatrix4fv(
                    m_projViewMatrixLoc, 1, GL_FALSE, projViewMatrix.mat
                );
            }

            ////////////////////////////////////////////////////////////////////
            //  Send model matrix                                             //
            ////////////////////////////////////////////////////////////////////
            inline void sendModelMatrix(Matrix4x4& modelMatrix)
            {
                glUniformMatrix4fv(
                    m_modelMatrixLoc, 1, GL_FALSE, modelMatrix.mat
                );
            }


            ////////////////////////////////////////////////////////////////////
            //  Send constants color uniform                                  //
            ////////////////////////////////////////////////////////////////////
            inline void sendColor(Vector4& color)
            {
                glUniform4fv(m_colorLoc, 1, color.vec);
            }

            ////////////////////////////////////////////////////////////////////
            //  Send constants time uniform                                   //
            ////////////////////////////////////////////////////////////////////
            inline void sendTime(float time)
            {
                glUniform1fv(m_timeLoc, 1, &time);
            }


            ////////////////////////////////////////////////////////////////////
            //  Get shader uniform location                                   //
            ////////////////////////////////////////////////////////////////////
            inline int32_t getUniform(const char* uniform)
            {
                return glGetUniformLocation(m_shader, uniform);
            }

            ////////////////////////////////////////////////////////////////////
            //  Send shader uniform value                                     //
            ////////////////////////////////////////////////////////////////////
            inline void sendUniform(int32_t location, float value)
            {
                glUniform1fv(location, 1, &value);
            }

            ////////////////////////////////////////////////////////////////////
            //  Send shader uniform integer value                             //
            ////////////////////////////////////////////////////////////////////
            inline void sendIntUniform(int32_t location, int32_t value)
            {
                glUniform1iv(location, 1, &value);
            }

            ////////////////////////////////////////////////////////////////////
            //  Send shader uniform vec 2                                     //
            ////////////////////////////////////////////////////////////////////
            inline void sendUniformVec2(int32_t location, Vector2& vec)
            {
                glUniform2fv(location, 1, vec.vec);
            }

            ////////////////////////////////////////////////////////////////////
            //  Send shader uniform vec 3                                     //
            ////////////////////////////////////////////////////////////////////
            inline void sendUniformVec3(int32_t location, Vector3& vec)
            {
                glUniform3fv(location, 1, vec.vec);
            }

            ////////////////////////////////////////////////////////////////////
            //  Send shader uniform vec 4                                     //
            ////////////////////////////////////////////////////////////////////
            inline void sendUniformVec4(int32_t location, Vector4& vec)
            {
                glUniform4fv(location, 1, vec.vec);
            }

            ////////////////////////////////////////////////////////////////////
            //  Send shader uniform mat 4                                     //
            ////////////////////////////////////////////////////////////////////
            inline void sendUniformMat4(int32_t location, Matrix4x4& mat)
            {
                glUniformMatrix4fv(location, 1, GL_FALSE, mat.mat);
            }


            ////////////////////////////////////////////////////////////////////
            //  Bind shader                                                   //
            ////////////////////////////////////////////////////////////////////
            void bind()
            {
                glUseProgram(m_shader);
            }

            ////////////////////////////////////////////////////////////////////
            //  Unbind shader                                                 //
            ////////////////////////////////////////////////////////////////////
            void unbind()
            {
                glUseProgram(0);
            }


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
            int32_t     m_shader;               // Shader handle
            int32_t     m_verticesLoc;          // Vertices location
            int32_t     m_texCoordsLoc;         // Texcoords locations
            int32_t     m_normalsLoc;           // Normals locations

            int32_t     m_projViewMatrixLoc;    // ProjView matrix location
            int32_t     m_modelMatrixLoc;       // Model matrix location

            int32_t     m_colorLoc;             // Color location
            int32_t     m_timeLoc;              // Time location
    };


#endif // WOS_RENDERER_SHADER_HEADER
