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
    #include "View.h"
    #include "Camera.h"
    #include "VertexBuffer.h"

    #include "Shaders/Default.h"
    #include "Shaders/Rectangle.h"
    #include "Shaders/Ellipse.h"
    #include "Shaders/StaticMesh.h"
    #include "Shaders/StaticProc.h"

    #include "../Resources/Resources.h"

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
    //  Renderer frame max ratios                                             //
    ////////////////////////////////////////////////////////////////////////////
    const bool RendererRatioMaxClamping = true;
    const float RendererRatioXMax = 2.0f;
    const float RendererRatioYMax = 0.7f;


    ////////////////////////////////////////////////////////////////////////////
    //  Renderer proj type enumaration                                        //
    ////////////////////////////////////////////////////////////////////////////
    enum RendererProjType
    {
        RENDERER_PROJ_VIEW = 0,
        RENDERER_PROJ_CAMERA = 1
    };


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
            //  Init renderer shaders                                         //
            //  return : True if the renderer shaders are ready               //
            ////////////////////////////////////////////////////////////////////
            bool initShaders();

            ////////////////////////////////////////////////////////////////////
            //  Start rendering frame                                         //
            //  return : True if the rendering frame is ready                 //
            ////////////////////////////////////////////////////////////////////
            bool startFrame();

            ////////////////////////////////////////////////////////////////////
            //  End rendering frame                                           //
            //  return : True if the frame is rendering                       //
            ////////////////////////////////////////////////////////////////////
            bool endFrame();


            ////////////////////////////////////////////////////////////////////
            //  Bind renderer shader                                          //
            ////////////////////////////////////////////////////////////////////
            inline void bindShader(RendererShader rendererShader)
            {
                shaders[rendererShader].bind();
                currentShader = &shaders[rendererShader];
                if (currentProjType == RENDERER_PROJ_CAMERA)
                {
                    currentCamera->bind();
                }
                else
                {
                    currentView->bind();
                }
            }

            ////////////////////////////////////////////////////////////////////
            //  Bind renderer shader                                          //
            ////////////////////////////////////////////////////////////////////
            inline void bindShader(Shader& shader)
            {
                shader.bind();
                currentShader = &shader;
                if (currentProjType == RENDERER_PROJ_CAMERA)
                {
                    currentCamera->bind();
                }
                else
                {
                    currentView->bind();
                }
            }

            ////////////////////////////////////////////////////////////////////
            //  Bind renderer vertex buffer                                   //
            ////////////////////////////////////////////////////////////////////
            inline void bindVertexBuffer(MeshesAssets meshAsset)
            {
                currentBuffer = &GResources.meshes.mesh(meshAsset);
            }


            ////////////////////////////////////////////////////////////////////
            //  Compute renderer default view                                 //
            ////////////////////////////////////////////////////////////////////
            inline void computeDefaultView()
            {
                defaultView.compute(getRatio());
            }

            ////////////////////////////////////////////////////////////////////
            //  Bind renderer default view                                    //
            ////////////////////////////////////////////////////////////////////
            inline void bindDefaultView()
            {
                currentView = &defaultView;
                currentProjType = RENDERER_PROJ_VIEW;
            }

            ////////////////////////////////////////////////////////////////////
            //  Bind renderer view                                            //
            ////////////////////////////////////////////////////////////////////
            inline void bindView(View& view)
            {
                currentView = &view;
                currentProjType = RENDERER_PROJ_VIEW;
            }

            ////////////////////////////////////////////////////////////////////
            //  Bind renderer camera                                          //
            ////////////////////////////////////////////////////////////////////
            inline void bindCamera(Camera& camera)
            {
                currentCamera = &camera;
                currentProjType = RENDERER_PROJ_CAMERA;
            }


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
            inline int getWidth()
            {
                return width;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer height                                           //
            //  return : Renderer height                                      //
            ////////////////////////////////////////////////////////////////////
            inline int getHeight()
            {
                return height;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer float width                                      //
            //  return : Renderer float width                                 //
            ////////////////////////////////////////////////////////////////////
            inline int getWidthF()
            {
                if (width <= 1) { return 1.0f; }
                return (width*1.0f);
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer float height                                     //
            //  return : Renderer float height                                //
            ////////////////////////////////////////////////////////////////////
            inline int getHeightF()
            {
                if (height <= 1) { return 1.0f; }
                return (height*1.0f);
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer X offset                                         //
            //  return : Renderer X offset                                    //
            ////////////////////////////////////////////////////////////////////
            inline int getOffsetX()
            {
                return offsetx;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer Y offset                                         //
            //  return : Renderer Y offset                                    //
            ////////////////////////////////////////////////////////////////////
            inline int getOffsetY()
            {
                return offsety;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer float X offset                                   //
            //  return : Renderer float X offset                              //
            ////////////////////////////////////////////////////////////////////
            inline float getOffsetXF()
            {
                return (offsetx*1.0f);
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer float Y offset                                   //
            //  return : Renderer float Y offset                              //
            ////////////////////////////////////////////////////////////////////
            inline float getOffsetYF()
            {
                return (offsety*1.0f);
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer scale                                            //
            //  return : Renderer scale (1/height)                            //
            ////////////////////////////////////////////////////////////////////
            inline float getScale()
            {
                if (height > 0.0f)
                {
                    return (1.0f/(height*1.0f));
                }
                return 1.0f;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get renderer aspect ratio                                     //
            //  return : Renderer aspect ratio                                //
            ////////////////////////////////////////////////////////////////////
            inline float getRatio()
            {
                if (height > 0)
                {
                    return ((width*1.0f)/(height*1.0f));
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
            int                 width;              // Renderer width
            int                 height;             // Renderer height
            int                 offsetx;            // Renderer X offset
            int                 offsety;            // Renderer Y offset

            Shader*             shaders;            // Shaders
            View                defaultView;        // Default view

            RendererProjType    currentProjType;    // Current proj type
            Shader*             currentShader;      // Current shader
            View*               currentView;        // Current view
            Camera*             currentCamera;      // Current camera
            VertexBuffer*       currentBuffer;      // Current vertex buffer
    };


    ////////////////////////////////////////////////////////////////////////////
    //  Renderer global instance                                              //
    ////////////////////////////////////////////////////////////////////////////
    extern Renderer GRenderer;


#endif // WOS_RENDERER_RENDERER_HEADER
