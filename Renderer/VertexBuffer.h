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
//     Renderer/VertexBuffer.h : VertexBuffer management                      //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_VERTEXBUFFER_HEADER
#define WOS_RENDERER_VERTEXBUFFER_HEADER

    #include <GLES2/gl2.h>

    #include "../System/System.h"

    #include <cstdint>


    ////////////////////////////////////////////////////////////////////////////
    //  Vertex inputs types enumeration                                       //
    ////////////////////////////////////////////////////////////////////////////
    enum VertexInputsType
    {
        VERTEX_INPUTS_DEFAULT = 0,
        VERTEX_INPUTS_CUBEMAP = 1,
        VERTEX_INPUTS_STATICMESH = 2
    };


    ////////////////////////////////////////////////////////////////////////////
    //  Default vertex buffer vertices                                        //
    ////////////////////////////////////////////////////////////////////////////
    const uint32_t DefaultVerticesCount = 20;
    const float DefaultVertices[] = {
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,  1.0f, 1.0f,
        0.5f, 0.5f, 0.0f,  1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,  0.0f, 0.0f
    };

    ////////////////////////////////////////////////////////////////////////////
    //  Default vertex buffer indices                                         //
    ////////////////////////////////////////////////////////////////////////////
    const uint32_t DefaultIndicesCount = 6;
    const uint32_t DefaultIndices[] = {
        0, 1, 2,
        2, 3, 0
    };


    ////////////////////////////////////////////////////////////////////////////
    //  VertexBuffer class definition                                         //
    ////////////////////////////////////////////////////////////////////////////
    class VertexBuffer
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  VertexBuffer default constructor                              //
            ////////////////////////////////////////////////////////////////////
            VertexBuffer();

            ////////////////////////////////////////////////////////////////////
            //  VertexBuffer destructor                                       //
            ////////////////////////////////////////////////////////////////////
            ~VertexBuffer();


            ////////////////////////////////////////////////////////////////////
            //  Create vertex buffer                                          //
            ////////////////////////////////////////////////////////////////////
            bool createBuffer();

            ////////////////////////////////////////////////////////////////////
            //  Create vertex buffer                                          //
            ////////////////////////////////////////////////////////////////////
            bool createBuffer(
                const float* vertices, const unsigned int* indices,
                uint32_t verticesCount, uint32_t indicesCount,
                VertexInputsType vertexInputType = VERTEX_INPUTS_DEFAULT);

            ////////////////////////////////////////////////////////////////////
            //  Update vertex buffer                                          //
            ////////////////////////////////////////////////////////////////////
            bool updateBuffer(
                const float* vertices, const unsigned int* indices,
                uint32_t verticesCount, uint32_t indicesCount,
                VertexInputsType vertexInputType = VERTEX_INPUTS_DEFAULT);

            ////////////////////////////////////////////////////////////////////
            //  Destroy vertex buffer                                         //
            ////////////////////////////////////////////////////////////////////
            void destroyBuffer();


            ////////////////////////////////////////////////////////////////////
            //  Render Vertex buffer                                          //
            ////////////////////////////////////////////////////////////////////
            void render();


        private:
            ////////////////////////////////////////////////////////////////////
            //  VertexBuffer private copy constructor : Not copyable          //
            ////////////////////////////////////////////////////////////////////
            VertexBuffer(const VertexBuffer&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  VertexBuffer private copy operator : Not copyable             //
            ////////////////////////////////////////////////////////////////////
            VertexBuffer& operator=(const VertexBuffer&) = delete;


        public:
            VertexInputsType    vertexType;         // Vertex input type
            uint32_t            vertexBuffer;       // Vertex buffer handle
            uint32_t            elementBuffer;      // Element buffer handle
            uint32_t            indicesRender;      // Indices render count
    };


#endif // WOS_RENDERER_VERTEXBUFFER_HEADER
