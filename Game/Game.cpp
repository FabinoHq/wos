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
//     Game/Game.cpp : Main game class management                             //
////////////////////////////////////////////////////////////////////////////////
#include "Game.h"


////////////////////////////////////////////////////////////////////////////////
//  Game default constructor                                                  //
////////////////////////////////////////////////////////////////////////////////
Game::Game() :
m_view(),
m_camera(),
m_sprite(),
m_procSprite(),
m_rectangle(),
m_ellipse(),
m_cuboid(),
m_staticmesh(),
m_mouseX(0.0f),
m_mouseY(0.0f)
{

}

////////////////////////////////////////////////////////////////////////////////
//  Game destructor                                                           //
////////////////////////////////////////////////////////////////////////////////
Game::~Game()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Init Game                                                                 //
//  return : True if game is ready, false otherwise                           //
////////////////////////////////////////////////////////////////////////////////
bool Game::init()
{
    // Set current thread as current context
    GSysWindow.setThread();

    // Init view
    if (!m_view.init())
    {
        // Could not init view
        GSysWindow.releaseThread();
        return false;
    }

    // Init camera
    if (!m_camera.init())
    {
        // Could not init camera
        GSysWindow.releaseThread();
        return false;
    }
    m_camera.setZ(2.0f);

    // Init sprite
    if (!m_sprite.init(GResources.textures.high(TEXTURE_TEST), 0.5f, 0.5f))
    {
        // Could not init sprite
        GSysWindow.releaseThread();
        return false;
    }

    // Init procedural sprite
    if (!m_procSprite.init(1.0f, 1.0f))
    {
        // Could not init procedural sprite
        GSysWindow.releaseThread();
        return false;
    }

    // Init rectangle shape
    if (!m_rectangle.init(1.0f, 1.0f))
    {
        // Could not init rectangle shape
        GSysWindow.releaseThread();
        return false;
    }

    // Init ellipse shape
    if (!m_ellipse.init(1.0f, 1.0f))
    {
        // Could not init ellipse shape
        GSysWindow.releaseThread();
        return false;
    }

    // Init cuboid shape
    if (!m_cuboid.init())
    {
        // Could not init cuboid shape
        GSysWindow.releaseThread();
        return false;
    }

    // Init static mesh
    if (!m_staticmesh.init(GResources.meshes.mesh(MESHES_TEST),
        GResources.textures.high(TEXTURE_TEST)))
    {
        // Could not init static mesh
        GSysWindow.releaseThread();
        return false;
    }

    // Game is ready
    GSysWindow.releaseThread();
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Compute game events                                                       //
////////////////////////////////////////////////////////////////////////////////
void Game::events(Event& event)
{
    // Get renderer scale and ratio
    float scale = GRenderer.getScale();
    float ratio = GRenderer.getRatio();

    // Process event
    switch (event.type)
    {
        // Key pressed
        case EVENT_KEYPRESSED:
            switch (event.key)
            {
                case EVENT_KEY_Z:
                    break;

                case EVENT_KEY_S:
                    break;

                case EVENT_KEY_Q:
                    break;

                case EVENT_KEY_D:
                    break;

                default:
                    break;
            }
            break;

        // Key released
        case EVENT_KEYRELEASED:
            switch (event.key)
            {
                case EVENT_KEY_Z:
                    break;

                case EVENT_KEY_S:
                    break;

                case EVENT_KEY_Q:
                    break;

                case EVENT_KEY_D:
                    break;

                default:
                    break;
            }
            break;

        // Mouse moved
        case EVENT_MOUSEMOVED:
            m_mouseX = (
                ((event.mouse.x-GRenderer.getOffsetXF())/
                (GRenderer.getWidthF()*0.5f))*ratio
            )-ratio;
            m_mouseY = (
                -(event.mouse.y-GRenderer.getOffsetYF())/
                (GRenderer.getHeightF()*0.5f)
            )+1.0f;
            if (m_mouseX <= -ratio) { m_mouseX = -ratio; }
            if (m_mouseX >= ratio) { m_mouseX = ratio; }
            if (m_mouseY <= -1.0f) { m_mouseY = -1.0f; }
            if (m_mouseY >= 1.0f) { m_mouseY = 1.0f; }
            break;

        // Mouse button pressed
        case EVENT_MOUSEPRESSED:
            if (event.mouse.button == EVENT_MOUSE_LEFT)
            {

            }
            break;

        // Mouse button released
        case EVENT_MOUSERELEASED:
            if (event.mouse.button == EVENT_MOUSE_LEFT)
            {

            }
            break;

        // Mouse wheel
        case EVENT_MOUSEWHEEL:
            break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Compute game logic                                                        //
////////////////////////////////////////////////////////////////////////////////
void Game::compute(float frametime)
{
    // Get render ratio
    float ratio = GRenderer.getRatio();

    // Compute views
    GRenderer.computeDefaultView();
    m_view.compute(ratio);

    // Compute cameras
    m_camera.compute(ratio);

    // Compute procedural sprite
    m_procSprite.setSize(ratio*2.0f, 2.0f);
    m_procSprite.centerOrigin();

    // Compute sprite
    m_sprite.rotate(frametime*0.5f);

    // Compute cuboid shape
    m_cuboid.rotateX(frametime*0.47f);
    m_cuboid.rotateY(frametime*0.21f);

    // Compute static mesh
    m_staticmesh.rotateX(frametime*0.47f);
    m_staticmesh.rotateY(frametime*0.21f);
}

////////////////////////////////////////////////////////////////////////////////
//  Render game                                                               //
////////////////////////////////////////////////////////////////////////////////
void Game::render()
{
    // Start frame rendering
    if (!GRenderer.startFrame())
    {
        return;
    }


    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Bind camera
    GRenderer.bindCamera(m_camera);

    // Render cuboid shape
    /*GRenderer.bindShader(RENDERER_SHADER_SHAPE);
    GRenderer.bindVertexBuffer(MESHES_CUBOID);
    m_cuboid.render();*/

    // Render static mesh
    GRenderer.bindShader(RENDERER_SHADER_STATICMESH);
    GRenderer.bindVertexBuffer(MESHES_TEST);
    m_staticmesh.bindTexture();
    m_staticmesh.render();


    // Disable depth test
    glDisable(GL_DEPTH_TEST);
    // Disable back face culling
    glDisable(GL_CULL_FACE);

    // Bind default vertex buffer
    GRenderer.bindVertexBuffer(MESHES_DEFAULT);
    // Bind default view
    GRenderer.bindDefaultView();

    // Render procedural sprite
    /*m_procSprite.bindShader();
    m_procSprite.setColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_procSprite.render();*/

    // Render sprite
    /*GRenderer.bindShader(RENDERER_SHADER_DEFAULT);
    m_sprite.bindTexture();
    m_sprite.render();*/

    // Render rectangle shape
    /*GRenderer.bindShader(RENDERER_SHADER_RECTANGLE);
    m_rectangle.render();*/

    // Render ellipse shape
    /*GRenderer.bindShader(RENDERER_SHADER_ELLIPSE);
    m_ellipse.render();*/


    // End frame rendering
    GRenderer.endFrame();
}
