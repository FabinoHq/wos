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
m_sprite(),
m_procSprite(),
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
    GRenderer.defaultView.compute(ratio);
    m_view.compute(ratio);

    // Compute procedural sprite
    m_procSprite.setSize(ratio*2.0f, 2.0f);
    m_procSprite.centerOrigin();

    // Compute sprite
    m_sprite.rotate(frametime*0.5f);
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

    // Render procedural sprite
    m_procSprite.bindShader();
    GRenderer.bindView(GRenderer.defaultView);
    m_procSprite.render();

    // Render sprite
    GRenderer.bindShader(RENDERER_SHADER_DEFAULT);
    GRenderer.bindView(GRenderer.defaultView);
    m_sprite.setTexture(GResources.textures.high(TEXTURE_TEST));
    m_sprite.bindTexture();
    m_sprite.render();

    // End frame rendering
    GRenderer.endFrame();
}
