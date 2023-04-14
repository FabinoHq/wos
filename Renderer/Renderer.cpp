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
width(0),
height(0),
offsetx(0),
offsety(0),
defaultShader(),
defaultProcShader(),
vertexBuffer(),
view()
{

}

////////////////////////////////////////////////////////////////////////////////
//  Renderer destructor                                                       //
////////////////////////////////////////////////////////////////////////////////
Renderer::~Renderer()
{

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
    /*if (!GSysWindow.isValid())
    {
        // Invalid SysWindow
        SysMessage::box() << "[0x3001] Invalid system window\n";
        SysMessage::box() << "System window must be valid";
        return false;
    }*/

    // Set renderer size
    int windowWidth = GSysWindow.getWidth();
    int windowHeight = GSysWindow.getHeight();
    width = windowWidth;
    height = windowHeight;
    offsetx = 0;
    offsety = 0;

    // Aspect ratio clamping
    if (RendererRatioMaxClamping)
    {
        if (width >= static_cast<int>(width*RendererRatioXMax))
        {
            width = static_cast<int>(width*RendererRatioXMax);
        }
        if (height >= static_cast<int>(height*RendererRatioYMax))
        {
            height = static_cast<int>(height*RendererRatioYMax);
        }
    }
    if (width <= 1) { width = 1; }
    if (height <= 1) { height = 1; }
    offsetx = static_cast<int>((windowWidth-width)*0.5f);
    offsety = static_cast<int>((windowHeight-height)*0.5f);

    // Create default shader
    if (!defaultShader.createShader(
        DefaultVertexShaderSrc, DefaultFragmentShaderSrc))
    {
        // Unable to create default shader
        SysMessage::box() << "[0x3002] Unable to create default shader\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // Create default procedural shader
    if (!defaultProcShader.createShader(
        DefaultVertexShaderSrc, DefaultProcFragmentShaderSrc))
    {
        // Unable to create default procedural shader
        SysMessage::box() << "[0x3002] Unable to create default proc shader\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // Create default vertex buffer
    if (!vertexBuffer.createBuffer())
    {
        // Unable to create default vertex buffer
        SysMessage::box() << "[0x3003] Unable to create vertex buffer\n";
        SysMessage::box() << "Please update your graphics drivers";
        return false;
    }

    // Create default view
    if (!view.init())
    {
        // Unable to create view
        SysMessage::box() << "[0x3004] Unable to create view\n";
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

    // Set texture 0 as active texture
    glActiveTexture(GL_TEXTURE0);

    // Disable byte alignment
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Clear renderer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Renderer successfully loaded
    ready = true;
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Start rendering frame                                                     //
//  return : True if the rendering frame is ready                             //
////////////////////////////////////////////////////////////////////////////////
bool Renderer::startFrame()
{
    // Set renderer size
    int windowWidth = GSysWindow.getWidth();
    int windowHeight = GSysWindow.getHeight();
    width = windowWidth;
    height = windowHeight;
    offsetx = 0;
    offsety = 0;

    // Aspect ratio clamping
    if (RendererRatioMaxClamping)
    {
        if (width >= static_cast<int>(width*RendererRatioXMax))
        {
            width = static_cast<int>(width*RendererRatioXMax);
        }
        if (height >= static_cast<int>(height*RendererRatioYMax))
        {
            height = static_cast<int>(height*RendererRatioYMax);
        }
    }
    if (width <= 1) { width = 1; }
    if (height <= 1) { height = 1; }
    offsetx = static_cast<int>((windowWidth-width)*0.5f);
    offsety = static_cast<int>((windowHeight-height)*0.5f);

    // Update viewport
    glViewport(offsetx, offsety, width, height);
    glScissor(offsetx, offsety, width, height);
    glDisable(GL_SCISSOR_TEST);

    // Clear frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Bind default shader
    defaultShader.bindShader();

    // Bind default view
    view.bind(defaultShader);

    // Rendering frame is ready
    return true;
}
