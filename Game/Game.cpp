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
m_freeflycam(),
m_orbitalcam(),
m_sprite(),
m_procSprite(),
m_rectangle(),
m_ellipse(),
m_cuboid(),
m_plane(),
m_cursor(),
m_pxText(),
m_guiWindow(),
m_staticmesh()
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

    // Init game
    if (!initGame())
    {
        // Could not init game
        GSysWindow.releaseThread();
        return false;
    }

    // Game is ready
    GSysWindow.releaseThread();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Init game                                                                 //
//  return : True if game is ready, false otherwise                           //
////////////////////////////////////////////////////////////////////////////////
bool Game::initGame()
{
    // Init view
    if (!m_view.init())
    {
        // Could not init view
        return false;
    }

    // Init camera
    if (!m_camera.init())
    {
        // Could not init camera
        return false;
    }
    m_camera.setZ(2.0f);

    // Init free fly camera
    if (!m_freeflycam.init())
    {
        // Could not init free fly camera
        return false;
    }
    m_freeflycam.setZ(2.0f);

    // Init orbital camera
    if (!m_orbitalcam.init())
    {
        // Could not init orbital camera
        return false;
    }
    m_orbitalcam.setZ(2.0f);
    m_orbitalcam.setTarget(0.0f, 0.0f, 0.0f);
    m_orbitalcam.setDistance(2.0f);
    m_orbitalcam.setSpeed(50.0f);

    // Init sprite
    if (!m_sprite.init(GResources.textures.high(TEXTURE_TEST), 0.5f, 0.5f))
    {
        // Could not init sprite
        return false;
    }

    // Init procedural sprite
    if (!m_procSprite.init(1.0f, 1.0f))
    {
        // Could not init procedural sprite
        return false;
    }

    // Init rectangle shape
    if (!m_rectangle.init(1.0f, 1.0f))
    {
        // Could not init rectangle shape
        return false;
    }

    // Init ellipse shape
    if (!m_ellipse.init(1.0f, 1.0f))
    {
        // Could not init ellipse shape
        return false;
    }

    // Init cuboid shape
    if (!m_cuboid.init())
    {
        // Could not init cuboid shape
        return false;
    }

    // Init plane
    if (!m_plane.init(GResources.textures.high(TEXTURE_TEST), 1.0f, 1.0f))
    {
        // Could not init plane
        return false;
    }
    m_plane.setBillboard(PLANE_BILLBOARD_SPHERICAL);
    m_plane.setTarget(m_orbitalcam);


    // Init GUI cursor
    if (!m_cursor.init(GResources.textures.gui(TEXTURE_CURSOR), 64.0f))
    {
        // Could not init GUI cursor
        return false;
    }

    // Init test pixel text
    if (!m_pxText.init(GResources.textures.gui(TEXTURE_PIXELFONT), 0.04f))
    {
        // Could not init test pixel text
        return false;
    }
    m_pxText.setSmooth(0.2f);
    m_pxText.setText("FPS : 0");

    // Init GUI window
    if (!m_guiWindow.init(
        GResources.textures.gui(TEXTURE_WINDOW), 1.0f, 1.0f, 3.75f))
    {
        // Could not init GUI window
        return false;
    }


    // Init static mesh
    if (!m_staticmesh.init(GResources.meshes.mesh(MESHES_TEST),
        GResources.textures.high(TEXTURE_TEST)))
    {
        // Could not init static mesh
        return false;
    }


    // Game is ready
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Compute game events                                                       //
////////////////////////////////////////////////////////////////////////////////
void Game::events(Event& event)
{
    // Process event
    switch (event.type)
    {
        // Key pressed
        case EVENT_KEYPRESSED:
            switch (event.key)
            {
                case EVENT_KEY_Z:
                    //m_freeflycam.setForward(true);
                    break;

                case EVENT_KEY_S:
                    //m_freeflycam.setBackward(true);
                    break;

                case EVENT_KEY_Q:
                    //m_freeflycam.setLeftward(true);
                    break;

                case EVENT_KEY_D:
                    //m_freeflycam.setRightward(true);
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
                    //m_freeflycam.setForward(false);
                    break;

                case EVENT_KEY_S:
                    //m_freeflycam.setBackward(false);
                    break;

                case EVENT_KEY_Q:
                    //m_freeflycam.setLeftward(false);
                    break;

                case EVENT_KEY_D:
                    //m_freeflycam.setRightward(false);
                    break;

                default:
                    break;
            }
            break;

        // Mouse moved
        case EVENT_MOUSEMOVED:
        {
            // Compute mouse events
            //m_freeflycam.mouseMove(GSysMouse.deltaX, GSysMouse.deltaY);
            m_orbitalcam.mouseMove(GSysMouse.deltaX, GSysMouse.deltaY);
            m_guiWindow.mouseMove(GSysMouse.mouseX, GSysMouse.mouseY);

            #if (WOS_POINTERLOCK == 1)
                // GUI cursor
                m_cursor.setCursor(
                    m_guiWindow.updateCursor(GSysMouse.mouseX, GSysMouse.mouseY)
                );
            #else
                // System cursor
                GSysWindow.setCursor(
                    m_guiWindow.updateCursor(GSysMouse.mouseX, GSysMouse.mouseY)
                );
            #endif // WOS_POINTERLOCK
            break;
        }

        // Mouse button pressed
        case EVENT_MOUSEPRESSED:
            if (event.mouse.button == EVENT_MOUSE_LEFT)
            {
                m_orbitalcam.mousePress();
                m_guiWindow.mousePress(GSysMouse.mouseX, GSysMouse.mouseY);
            }
            break;

        // Mouse button released
        case EVENT_MOUSERELEASED:
            if (event.mouse.button == EVENT_MOUSE_LEFT)
            {
                m_orbitalcam.mouseRelease();
                m_guiWindow.mouseRelease(GSysMouse.mouseX, GSysMouse.mouseY);
            }
            break;

        // Mouse wheel
        case EVENT_MOUSEWHEEL:
            m_orbitalcam.mouseWheel(event.mouse.wheel);
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
    // Framerate
    static std::ostringstream framestr;
    static float frameavg = 0.0f;
    static float framecnt = 0.0f;

    // Framerate display
    frameavg += frametime;
    framecnt += 1.0f;
    if ((frameavg >= 0.5f) && (framecnt >= 1.0f))
    {
        framestr.clear();
        framestr.str("");
        framestr << "FPS : " << (1.0f/(frameavg/framecnt));
        frameavg = 0.0f;
        framecnt = 0.0f;
    }
    m_pxText.setText(framestr.str());


    // Get render ratio
    float ratio = GRenderer.getRatio();

    // Compute views
    GRenderer.computeDefaultView();
    m_view.compute(ratio);

    // Compute cameras
    m_camera.compute(ratio);
    m_freeflycam.compute(ratio, frametime);
    m_orbitalcam.compute(ratio, frametime);

    // Compute procedural sprite
    m_procSprite.setSize(ratio*2.0f, 2.0f);

    // Compute sprite
    m_sprite.rotate(frametime*0.5f);

    // Compute cuboid shape
    m_cuboid.rotateX(frametime*0.47f);
    m_cuboid.rotateY(frametime*0.21f);

    // Compute plane
    m_plane.setX(2.0f);

    // Compute static mesh
    /*m_staticmesh.rotateX(frametime*0.47f);
    m_staticmesh.rotateY(frametime*0.21f);*/
    m_staticmesh.setX(-2.0f);
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

    // Get render ratio
    float ratio = GRenderer.getRatio();


    // Enable depth test
    GRenderer.enableDepthTest();
    // Enable back face culling
    GRenderer.enableCullFace();

    // Bind camera
    m_orbitalcam.bind();

    // Render cuboid shape
    /*GRenderer.bindShader(RENDERER_SHADER_SHAPE);
    GRenderer.bindVertexBuffer(MESHES_CUBOID);
    m_cuboid.render();*/

    // Render plane
    GRenderer.bindShader(RENDERER_SHADER_STATICMESH);
    GRenderer.bindVertexBuffer(MESHES_PLANE);
    m_plane.bindTexture();
    m_plane.render();

    // Render static mesh
    GRenderer.bindShader(RENDERER_SHADER_STATICMESH);
    GRenderer.bindVertexBuffer(MESHES_TEST);
    m_staticmesh.bindTexture();
    m_staticmesh.render();


    // Disable depth test
    GRenderer.disableDepthTest();
    // Disable back face culling
    GRenderer.disableCullFace();

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

    // Render window
    GRenderer.bindShader(RENDERER_SHADER_NINEPATCH);
    m_guiWindow.bindTexture();
    m_guiWindow.render();

    // Render pixel text (framerate)
    GRenderer.bindShader(RENDERER_SHADER_PXTEXT);
    m_pxText.bindTexture();
    m_pxText.setPosition(-ratio+0.01f, 1.0f-(m_pxText.getHeight()*0.7f));
    m_pxText.render();

    // Render pixel text (camera position)
    std::ostringstream camerastr;
    camerastr << "X : " << m_orbitalcam.getX() <<
        " | Y : " << m_orbitalcam.getY() <<
        " | Z : " << m_orbitalcam.getZ();
    m_pxText.setText(camerastr.str());
    m_pxText.setPosition(-ratio+0.01f, 0.96f-(m_pxText.getHeight()*0.7f));
    m_pxText.render();

    // Render cursor
    GRenderer.bindShader(RENDERER_SHADER_DEFAULT);
    m_cursor.setPosition(GSysMouse.mouseX, GSysMouse.mouseY);
    m_cursor.bindTexture();
    m_cursor.render();


    // End frame rendering
    GRenderer.endFrame();
}
