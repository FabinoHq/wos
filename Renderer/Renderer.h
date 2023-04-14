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
//     Renderer/Renderer.h : Renderer management                              //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_RENDERER_HEADER
#define WOS_RENDERER_RENDERER_HEADER

    #include <GLES2/gl2.h>

    #include "../System/System.h"
    #include "../System/SysMessage.h"
    #include "../System/SysWindow.h"

    #include "../Math/Math.h"
    #include "../Math/Vector2.h"
    #include "../Math/Matrix4x4.h"

    #include "Shader.h"
    #include "VertexBuffer.h"
    #include "View.h"

    #include "Shaders/Default.h"
    #include "Shaders/DefaultProc.h"

    #include <cstddef>
    #include <cstdint>
    #include <vector>


    ////////////////////////////////////////////////////////////////////////////
    //  Renderer clear color                                                  //
    ////////////////////////////////////////////////////////////////////////////
    const float RendererClearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    ////////////////////////////////////////////////////////////////////////////
    //  Renderer compositing plane offset                                     //
    ////////////////////////////////////////////////////////////////////////////
    const float RendererCompositingPlaneOffset = 0.00001f;


    ////////////////////////////////////////////////////////////////////////////
    //  Renderer class definition                                             //
    ////////////////////////////////////////////////////////////////////////////
    class Renderer
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Renderer default constructor                                  //
            ////////////////////////////////////////////////////////////////////
            Renderer();

            ////////////////////////////////////////////////////////////////////
            //  Renderer destructor                                           //
            ////////////////////////////////////////////////////////////////////
            ~Renderer();


            ////////////////////////////////////////////////////////////////////
            //  Init renderer                                                 //
            //  return : True if the renderer is successfully loaded          //
            ////////////////////////////////////////////////////////////////////
            bool init();

            ////////////////////////////////////////////////////////////////////
            //  Start rendering frame                                         //
            //  return : True if the rendering frame is ready                 //
            ////////////////////////////////////////////////////////////////////
            bool startFrame();


            ////////////////////////////////////////////////////////////////////
            //  Get renderer ready state                                      //
            //  return : True if the renderer is ready, false otherwise       //
            ////////////////////////////////////////////////////////////////////
            inline bool isReady()
            {
                return ready;
            }


            ////////////////////////////////////////////////////////////////////
            //  Get renderer width                                            //
            //  return : Renderer width                                       //
            ////////////////////////////////////////////////////////////////////
            inline float getWidth()
            {
                return width;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer height                                           //
            //  return : Renderer height                                      //
            ////////////////////////////////////////////////////////////////////
            inline float getHeight()
            {
                return height;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer X offset                                         //
            //  return : Renderer X offset                                    //
            ////////////////////////////////////////////////////////////////////
            inline float getOffsetX()
            {
                return offsetx;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer Y offset                                         //
            //  return : Renderer Y offset                                    //
            ////////////////////////////////////////////////////////////////////
            inline float getOffsetY()
            {
                return offsety;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer scale                                            //
            //  return : Renderer scale (1/height)                            //
            ////////////////////////////////////////////////////////////////////
            inline float getScale()
            {
                if (height > 0.0f)
                {
                    return (1.0f/height);
                }
                return 1.0f;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer aspect ratio                                     //
            //  return : Renderer aspect ratio                                //
            ////////////////////////////////////////////////////////////////////
            inline float getRatio()
            {
                if (height > 0.0f)
                {
                    return (width/height);
                }
                return 1.0f;
            }


        private:
            ////////////////////////////////////////////////////////////////////
            //  Renderer private copy constructor : Not copyable              //
            ////////////////////////////////////////////////////////////////////
            Renderer(const Renderer&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Renderer private copy operator : Not copyable                 //
            ////////////////////////////////////////////////////////////////////
            Renderer& operator=(const Renderer&) = delete;


        public:
            bool                ready;              // Renderer ready state
            float               width;              // Renderer width
            float               height;             // Renderer height
            float               offsetx;            // Renderer X offset
            float               offsety;            // Renderer Y offset

            Shader              defaultShader;      // Default renderer shader
            Shader              defaultProcShader;  // Default proc shader
            VertexBuffer        vertexBuffer;       // Default vertex buffer
            View                view;               // Default view
    };


    ////////////////////////////////////////////////////////////////////////////
    //  Renderer global instance                                              //
    ////////////////////////////////////////////////////////////////////////////
    extern Renderer GRenderer;


#endif // WOS_RENDERER_RENDERER_HEADER
