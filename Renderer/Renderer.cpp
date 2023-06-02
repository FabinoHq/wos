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
//     Renderer/Renderer.cpp : Renderer management                            //
////////////////////////////////////////////////////////////////////////////////
#include "Renderer.h"


////////////////////////////////////////////////////////////////////////////////
//  Renderer global instance                                                  //
////////////////////////////////////////////////////////////////////////////////
Renderer GRenderer = Renderer();


////////////////////////////////////////////////////////////////////////////////
//  Renderer default constructor                                              //
////////////////////////////////////////////////////////////////////////////////
Renderer::Renderer() :
ready(false),
width(1),
height(1),
offsetx(0),
offsety(0),
shaders(0),
defaultView(),
currentShader(0),
currentView(0),
currentCamera(0),
currentBuffer(0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  Renderer destructor                                                       //
////////////////////////////////////////////////////////////////////////////////
Renderer::~Renderer()
{
    currentBuffer = 0;
    currentCamera = 0;
    currentView = 0;
    currentShader = 0;
    shaders = 0;
    offsety = 0;
    offsetx = 0;
    height = 0;
    width = 0;
    ready = false;
}


////////////////////////////////////////////////////////////////////////////////
//  Init renderer                                                             //
//  return : True if the renderer is successfully loaded                      //
////////////////////////////////////////////////////////////////////////////////
bool Renderer::init()
{
    // Init renderer
    ready = false;

    // Check SysWindow
    if (!GSysWindow.isValid())
    {
        // Invalid SysWindow
        SysMessage::box() << "[0x3001] Invalid system window\n";
        SysMessage::box() << "System window must be valid";
        return false;
    }

    // Set current thread as current context
    GSysWindow.setThread();

    // Set renderer size
    width = GSysWindow.getWidth();
    height = GSysWindow.getHeight();
    offsetx = 0;
    offsety = 0;

    // Aspect ratio clamping
    if (RendererRatioMaxClamping)
    {
        float vpwidth = (width*1.0f);
        float vpheight = (height*1.0f);
        float vpoffx = 0.0f;
        float vpoffy = 0.0f;

        if (vpwidth >= (vpheight*RendererRatioXMax))
        {
            vpwidth = (vpheight*RendererRatioXMax);
        }
        if (vpheight >= (vpwidth*RendererRatioYMax))
        {
            vpheight = (vpwidth*RendererRatioYMax);
        }

        if (vpwidth <= 1.0f) { vpwidth = 1.0f; }
        if (vpheight <= 1.0f) { vpheight = 1.0f; }
        vpoffx = ((width*1.0f)-vpwidth)*0.5f;
        vpoffy = ((height*1.0f)-vpheight)*0.5f;

        offsetx = static_cast<int>(std::ceil(vpoffx));
        offsety = static_cast<int>(std::ceil(vpoffy));
        width = static_cast<int>(std::ceil(vpwidth));
        height = static_cast<int>(std::ceil(vpheight));
    }
    if (offsetx <= 0) { offsetx = 0; }
    if (offsety <= 0) { offsety = 0; }
    if (width <= 1) { width = 1; }
    if (height <= 1) { height = 1; }

    // Init renderer shaders
    if (!initShaders())
    {
        // Could not init renderer shaders
        return false;
    }

    // Create default view
    if (!defaultView.init())
    {
        // Unable to create default view
        SysMessage::box() << "[0x3004] Unable to create default view\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // OpenGL settings
    glClearColor(
        RendererClearColor[0],
        RendererClearColor[1],
        RendererClearColor[2],
        RendererClearColor[3]
    );

    // Init viewport
    glViewport(offsetx, offsety, width, height);
    glScissor(offsetx, offsety, width, height);
    glDisable(GL_SCISSOR_TEST);

    // Disable back face culling
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Disable dithering
    glDisable(GL_DITHER);

    // Init depth and blend functions
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // Disable stencil
    glDisable(GL_STENCIL_TEST);

    // Set texture 0 as active texture
    glActiveTexture(GL_TEXTURE0);

    // Disable byte alignment
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Clear renderer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Renderer successfully loaded
    GSysWindow.releaseThread();
    GSysWindow.updateSize();
    ready = true;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Init renderer shaders                                                     //
//  return : True if the renderer shaders are ready                           //
////////////////////////////////////////////////////////////////////////////////
bool Renderer::initShaders()
{
    // Check shaders
    if (shaders)
    {
        // Shaders already allocated
        return false;
    }

    // Allocate shaders
    shaders = new (std::nothrow) Shader[RENDERER_SHADER_SHADERSCOUNT];
    if (!shaders)
    {
        // Could not allocate shaders
        return false;
    }


    // Create default shader
    if (!shaders[RENDERER_SHADER_DEFAULT].createShader(
        DefaultVertexShaderSrc, DefaultFragmentShaderSrc))
    {
        // Could not create default shader
        SysMessage::box() << "[0x3053] Could not create default shader\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // Create rectangle shader
    if (!shaders[RENDERER_SHADER_RECTANGLE].createShader(
        DefaultVertexShaderSrc, RectangleFragmentShaderSrc))
    {
        // Could not create default shader
        SysMessage::box() << "[0x3053] Could not create rectangle shader\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // Create ellipse shader
    if (!shaders[RENDERER_SHADER_ELLIPSE].createShader(
        DefaultVertexShaderSrc, EllipseFragmentShaderSrc))
    {
        // Could not create ellipse shader
        SysMessage::box() << "[0x3053] Could not create ellipse shader\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // Create shape shader
    if (!shaders[RENDERER_SHADER_SHAPE].createShader(
        StaticMeshVertexShaderSrc, StaticProcFragmentShaderSrc))
    {
        // Could not create static procedural shader
        SysMessage::box() << "[0x3053] Could not create static proc shader\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // Create static mesh shader
    if (!shaders[RENDERER_SHADER_STATICMESH].createShader(
        StaticMeshVertexShaderSrc, StaticMeshFragmentShaderSrc))
    {
        // Could not create static mesh shader
        SysMessage::box() << "[0x3053] Could not create static mesh shader\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // Renderer shaders are ready
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Start rendering frame                                                     //
//  return : True if the rendering frame is ready                             //
////////////////////////////////////////////////////////////////////////////////
bool Renderer::startFrame()
{
    // Set current thread as current context
    GSysWindow.setThread();

    // Set renderer size
    width = GSysWindow.getWidth();
    height = GSysWindow.getHeight();
    offsetx = 0;
    offsety = 0;

    // Aspect ratio clamping
    if (RendererRatioMaxClamping)
    {
        float vpwidth = (width*1.0f);
        float vpheight = (height*1.0f);
        float vpoffx = 0.0f;
        float vpoffy = 0.0f;

        if (vpwidth >= (vpheight*RendererRatioXMax))
        {
            vpwidth = (vpheight*RendererRatioXMax);
        }
        if (vpheight >= (vpwidth*RendererRatioYMax))
        {
            vpheight = (vpwidth*RendererRatioYMax);
        }

        if (vpwidth <= 1.0f) { vpwidth = 1.0f; }
        if (vpheight <= 1.0f) { vpheight = 1.0f; }
        vpoffx = ((width*1.0f)-vpwidth)*0.5f;
        vpoffy = ((height*1.0f)-vpheight)*0.5f;

        offsetx = static_cast<int>(std::ceil(vpoffx));
        offsety = static_cast<int>(std::ceil(vpoffy));
        width = static_cast<int>(std::ceil(vpwidth));
        height = static_cast<int>(std::ceil(vpheight));
    }
    if (offsetx <= 0) { offsetx = 0; }
    if (offsety <= 0) { offsety = 0; }
    if (width <= 1) { width = 1; }
    if (height <= 1) { height = 1; }

    // Update viewport
    glViewport(offsetx, offsety, width, height);
    glScissor(offsetx, offsety, width, height);
    glDisable(GL_SCISSOR_TEST);

    // Clear frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind default vertex buffer
    bindVertexBuffer(MESHES_DEFAULT);

    // Bind default view
    bindView(defaultView);

    // Bind default shader
    bindShader(RENDERER_SHADER_DEFAULT);

    // Rendering frame is ready
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  End rendering frame                                                       //
//  return : True if the frame is rendering                                   //
////////////////////////////////////////////////////////////////////////////////
bool Renderer::endFrame()
{
    // Check renderer state
    if (!ready)
    {
        // Renderer is not ready
        return false;
    }

    // Release current thread from current context
    GSysWindow.releaseThread();

    // Current frame is submitted for rendering
    return true;
}
