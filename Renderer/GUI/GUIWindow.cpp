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
//     Renderer/GUI/GUIWindow.cpp : GUI Window management                     //
////////////////////////////////////////////////////////////////////////////////
#include "GUIWindow.h"
#include "../Renderer.h"


////////////////////////////////////////////////////////////////////////////////
//  GUIWindow default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
GUIWindow::GUIWindow() :
Transform2(),
m_texture(0),
m_color(1.0f, 1.0f, 1.0f, 1.0f),
m_uvFactor(1.0f),
m_movable(true),
m_resizable(true),
m_minSize(GUIWindowDefaultMinSize),
m_maxSize(GUIWindowDefaultMaxSize),
m_topBarSize(GUIWindowDefaultTopBarSize),
m_resizeBarSize(GUIWindowDefaultResizeBarSize),
m_grabWindow(false),
m_grabTop(false),
m_grabBottom(false),
m_grabLeft(false),
m_grabRight(false),
m_grabVector(0.0f, 0.0f)
{

}

////////////////////////////////////////////////////////////////////////////////
//  GUIWindow virtual destructor                                              //
////////////////////////////////////////////////////////////////////////////////
GUIWindow::~GUIWindow()
{
    m_grabVector.reset();
    m_grabRight = false;
    m_grabLeft = false;
    m_grabBottom = false;
    m_grabTop = false;
    m_grabWindow = false;
    m_resizeBarSize = 0.0f;
    m_topBarSize = 0.0f;
    m_maxSize.reset();
    m_minSize.reset();
    m_resizable = false;
    m_movable = false;
    m_uvFactor = 0.0f;
    m_color.reset();
}


////////////////////////////////////////////////////////////////////////////////
//  Init window                                                               //
//  return : True if the window is successfully created                       //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::init(Texture& texture,
    float width, float height, float uvFactor)
{
    // Check texture handle
    if (!texture.isValid())
    {
        // Invalid texture handle
        return false;
    }

    // Reset window transformations
    resetTransforms();

    // Set window texture pointer
    m_texture = &texture;

    // Reset window color
    m_color.set(1.0f, 1.0f, 1.0f, 1.0f);

    // Set ninepatch UV factor
    m_uvFactor = uvFactor;

    // Reset window movable state
    m_movable = true;

    // Reset window resizable state
    m_resizable = true;

    // Reset window minimum size
    m_minSize.set(GUIWindowDefaultMinSize);

    // Reset window maximum size
    m_maxSize.set(GUIWindowDefaultMaxSize);

    // Reset window top bar size
    m_topBarSize = GUIWindowDefaultTopBarSize;

    // Reset window resize bar size
    m_resizeBarSize = GUIWindowDefaultResizeBarSize;

    // Set window size
    setSize(width, height);
    clampWindowSize();

    // Reset window grabbing states
    m_grabWindow = false;
    m_grabTop = false;
    m_grabBottom = false;
    m_grabLeft = false;
    m_grabRight = false;
    m_grabVector.reset();

    // Window successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Set window texture                                                        //
//  return : True if window texture is successfully set                       //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::setTexture(Texture& texture)
{
    // Check texture handle
    if (!texture.isValid())
    {
        // Invalid texture handle
        return false;
    }

    // Set window texture pointer
    m_texture = &texture;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Set window color                                                          //
////////////////////////////////////////////////////////////////////////////////
void GUIWindow::setColor(const Vector4& color)
{
    m_color.vec[0] = color.vec[0];
    m_color.vec[1] = color.vec[1];
    m_color.vec[2] = color.vec[2];
    m_color.vec[3] = color.vec[3];
}

////////////////////////////////////////////////////////////////////////////////
//  Set window color                                                          //
////////////////////////////////////////////////////////////////////////////////
void GUIWindow::setColor(float red, float green, float blue, float alpha)
{
    m_color.vec[0] = red;
    m_color.vec[1] = green;
    m_color.vec[2] = blue;
    m_color.vec[3] = alpha;
}


////////////////////////////////////////////////////////////////////////////////
//  Get window picking state                                                  //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::isPicking(float mouseX, float mouseY)
{
    return (
        (mouseX >= (m_position.vec[0] - m_size.vec[0]*0.5f)) &&
        (mouseX <= (m_position.vec[0] + m_size.vec[0]*0.5f)) &&
        (mouseY >= (m_position.vec[1] - m_size.vec[1]*0.5f)) &&
        (mouseY <= (m_position.vec[1] + m_size.vec[1]*0.5f))
    );
}

////////////////////////////////////////////////////////////////////////////////
//  Get window top bar picking state                                          //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::isTopBarPicking(float mouseX, float mouseY)
{
    return (
        (mouseX >= (m_position.vec[0] - m_size.vec[0]*0.5f)) &&
        (mouseX <= (m_position.vec[0] + m_size.vec[0]*0.5f)) &&
        (mouseY >= (m_position.vec[1] + m_size.vec[1]*0.5f-m_topBarSize)) &&
        (mouseY <= (m_position.vec[1] + m_size.vec[1]*0.5f))
    );
}

////////////////////////////////////////////////////////////////////////////////
//  Get window top resize bar picking state                                   //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::isTopResizePicking(float mouseX, float mouseY)
{
    return (
        (mouseX >= (m_position.vec[0] - m_size.vec[0]*0.5f)) &&
        (mouseX <= (m_position.vec[0] + m_size.vec[0]*0.5f)) &&
        (mouseY >= (m_position.vec[1] + m_size.vec[1]*0.5f-m_resizeBarSize)) &&
        (mouseY <= (m_position.vec[1] + m_size.vec[1]*0.5f))
    );
}

////////////////////////////////////////////////////////////////////////////////
//  Get window bottom resize bar picking state                                //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::isBottomResizePicking(float mouseX, float mouseY)
{
    return (
        (mouseX >= (m_position.vec[0] - m_size.vec[0]*0.5f)) &&
        (mouseX <= (m_position.vec[0] + m_size.vec[0]*0.5f)) &&
        (mouseY >= (m_position.vec[1] - m_size.vec[1]*0.5f)) &&
        (mouseY <= (m_position.vec[1] - m_size.vec[1]*0.5f+m_resizeBarSize))
    );
}

////////////////////////////////////////////////////////////////////////////////
//  Get window left resize bar picking state                                  //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::isLeftResizePicking(float mouseX, float mouseY)
{
    return (
        (mouseX >= (m_position.vec[0] - m_size.vec[0]*0.5f)) &&
        (mouseX <= (m_position.vec[0] - m_size.vec[0]*0.5f+m_resizeBarSize)) &&
        (mouseY >= (m_position.vec[1] - m_size.vec[1]*0.5f)) &&
        (mouseY <= (m_position.vec[1] + m_size.vec[1]*0.5f))
    );
}

////////////////////////////////////////////////////////////////////////////////
//  Get window right resize bar picking state                                 //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::isRightResizePicking(float mouseX, float mouseY)
{
    return (
        (mouseX >= (m_position.vec[0] + m_size.vec[0]*0.5f-m_resizeBarSize)) &&
        (mouseX <= (m_position.vec[0] + m_size.vec[0]*0.5f)) &&
        (mouseY >= (m_position.vec[1] - m_size.vec[1]*0.5f)) &&
        (mouseY <= (m_position.vec[1] + m_size.vec[1]*0.5f))
    );
}


////////////////////////////////////////////////////////////////////////////////
//  Handle window mouse press event                                           //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::mousePress(float mouseX, float mouseY)
{
    if (m_resizable)
    {
        if (isTopResizePicking(mouseX, mouseY) &&
            isLeftResizePicking(mouseX, mouseY))
        {
            // Top Left resize
            m_grabVector.vec[0] = (m_size.vec[0] + mouseX);
            m_grabVector.vec[1] = (m_size.vec[1] - mouseY);
            m_grabTop = true;
            m_grabLeft = true;
            m_grabWindow = false;
            m_grabBottom = false;
            m_grabRight = false;
            return true;
        }

        if (isTopResizePicking(mouseX, mouseY) &&
            isRightResizePicking(mouseX, mouseY))
        {
            // Top Right resize
            m_grabVector.vec[0] = (m_size.vec[0] - mouseX);
            m_grabVector.vec[1] = (m_size.vec[1] - mouseY);
            m_grabTop = true;
            m_grabRight = true;
            m_grabWindow = false;
            m_grabBottom = false;
            m_grabLeft = false;
            return true;
        }

        if (isBottomResizePicking(mouseX, mouseY) &&
            isLeftResizePicking(mouseX, mouseY))
        {
            // Bottom Left resize
            m_grabVector.vec[0] = (m_size.vec[0] + mouseX);
            m_grabVector.vec[1] = (m_size.vec[1] + mouseY);
            m_grabBottom = true;
            m_grabLeft = true;
            m_grabWindow = false;
            m_grabTop = false;
            m_grabRight = false;
            return true;
        }

        if (isBottomResizePicking(mouseX, mouseY) &&
            isRightResizePicking(mouseX, mouseY))
        {
            // Bottom Right resize
            m_grabVector.vec[0] = (m_size.vec[0] - mouseX);
            m_grabVector.vec[1] = (m_size.vec[1] + mouseY);
            m_grabBottom = true;
            m_grabRight = true;
            m_grabWindow = false;
            m_grabTop = false;
            m_grabLeft = false;
            return true;
        }

        if (isTopResizePicking(mouseX, mouseY))
        {
            // Top resize
            m_grabVector.vec[1] = (m_size.vec[1] - mouseY);
            m_grabTop = true;
            m_grabWindow = false;
            m_grabBottom = false;
            m_grabLeft = false;
            m_grabRight = false;
            return true;
        }

        if (isBottomResizePicking(mouseX, mouseY))
        {
            // Bottom resize
            m_grabVector.vec[1] = (m_size.vec[1] + mouseY);
            m_grabBottom = true;
            m_grabWindow = false;
            m_grabTop = false;
            m_grabLeft = false;
            m_grabRight = false;
            return true;
        }

        if (isLeftResizePicking(mouseX, mouseY))
        {
            // Left resize
            m_grabVector.vec[0] = (m_size.vec[0] + mouseX);
            m_grabLeft = true;
            m_grabWindow = false;
            m_grabTop = false;
            m_grabBottom = false;
            m_grabRight = false;
            return true;
        }

        if (isRightResizePicking(mouseX, mouseY))
        {
            // Right resize
            m_grabVector.vec[0] = (m_size.vec[0] - mouseX);
            m_grabRight = true;
            m_grabWindow = false;
            m_grabTop = false;
            m_grabBottom = false;
            m_grabLeft = false;
            return true;
        }
    }

    if (m_movable && isTopBarPicking(mouseX, mouseY))
    {
        // Move window
        m_grabVector.vec[0] = (m_position.vec[0] - mouseX);
        m_grabVector.vec[1] = (m_position.vec[1] - mouseY);
        m_grabWindow = true;
        m_grabTop = false;
        m_grabBottom = false;
        m_grabLeft = false;
        m_grabRight = false;
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
//  Handle window mouse release event                                         //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::mouseRelease(float mouseX, float mouseY)
{
    m_grabWindow = false;
    m_grabTop = false;
    m_grabBottom = false;
    m_grabLeft = false;
    m_grabRight = false;
    return isPicking(mouseX, mouseY);
}

////////////////////////////////////////////////////////////////////////////////
//  Handle window mouse move event                                            //
////////////////////////////////////////////////////////////////////////////////
bool GUIWindow::mouseMove(float mouseX, float mouseY)
{
    Vector2 moveOffset(0.0f, 0.0f);

    if (m_grabTop && m_grabLeft)
    {
        // Resize top left window
        moveOffset.vec[0] = m_size.vec[0]*0.5f;
        moveOffset.vec[1] = m_size.vec[1]*0.5f;
        m_size.vec[0] = (m_grabVector.vec[0] - mouseX);
        m_size.vec[1] = (m_grabVector.vec[1] + mouseY);
        clampWindowSize();
        moveOffset.vec[0] -= m_size.vec[0]*0.5f;
        m_position.vec[0] += moveOffset.vec[0];
        moveOffset.vec[1] -= m_size.vec[1]*0.5f;
        m_position.vec[1] -= moveOffset.vec[1];
        return true;
    }

    if (m_grabTop && m_grabRight)
    {
        // Resize top right window
        moveOffset.vec[0] = m_size.vec[0]*0.5f;
        moveOffset.vec[1] = m_size.vec[1]*0.5f;
        m_size.vec[0] = (m_grabVector.vec[0] + mouseX);
        m_size.vec[1] = (m_grabVector.vec[1] + mouseY);
        clampWindowSize();
        moveOffset.vec[0] -= m_size.vec[0]*0.5f;
        m_position.vec[0] -= moveOffset.vec[0];
        moveOffset.vec[1] -= m_size.vec[1]*0.5f;
        m_position.vec[1] -= moveOffset.vec[1];
        return true;
    }

    if (m_grabBottom && m_grabLeft)
    {
        // Resize bottom left window
        moveOffset.vec[0] = m_size.vec[0]*0.5f;
        moveOffset.vec[1] = m_size.vec[1]*0.5f;
        m_size.vec[0] = (m_grabVector.vec[0] - mouseX);
        m_size.vec[1] = (m_grabVector.vec[1] - mouseY);
        clampWindowSize();
        moveOffset.vec[0] -= m_size.vec[0]*0.5f;
        m_position.vec[0] += moveOffset.vec[0];
        moveOffset.vec[1] -= m_size.vec[1]*0.5f;
        m_position.vec[1] += moveOffset.vec[1];
        return true;
    }

    if (m_grabBottom && m_grabRight)
    {
        // Resize bottom right window
        moveOffset.vec[0] = m_size.vec[0]*0.5f;
        moveOffset.vec[1] = m_size.vec[1]*0.5f;
        m_size.vec[0] = (m_grabVector.vec[0] + mouseX);
        m_size.vec[1] = (m_grabVector.vec[1] - mouseY);
        clampWindowSize();
        moveOffset.vec[0] -= m_size.vec[0]*0.5f;
        m_position.vec[0] -= moveOffset.vec[0];
        moveOffset.vec[1] -= m_size.vec[1]*0.5f;
        m_position.vec[1] += moveOffset.vec[1];
        return true;
    }

    if (m_grabTop)
    {
        // Resize top window
        moveOffset.vec[1] = m_size.vec[1]*0.5f;
        m_size.vec[1] = (m_grabVector.vec[1] + mouseY);
        clampWindowSize();
        moveOffset.vec[1] -= m_size.vec[1]*0.5f;
        m_position.vec[1] -= moveOffset.vec[1];
        return true;
    }

    if (m_grabBottom)
    {
        // Resize bottom window
        moveOffset.vec[1] = m_size.vec[1]*0.5f;
        m_size.vec[1] = (m_grabVector.vec[1] - mouseY);
        clampWindowSize();
        moveOffset.vec[1] -= m_size.vec[1]*0.5f;
        m_position.vec[1] += moveOffset.vec[1];
        return true;
    }

    if (m_grabLeft)
    {
        // Resize left window
        moveOffset.vec[0] = m_size.vec[0]*0.5f;
        m_size.vec[0] = (m_grabVector.vec[0] - mouseX);
        clampWindowSize();
        moveOffset.vec[0] -= m_size.vec[0]*0.5f;
        m_position.vec[0] += moveOffset.vec[0];
        return true;
    }

    if (m_grabRight)
    {
        // Resize right window
        moveOffset.vec[0] = m_size.vec[0]*0.5f;
        m_size.vec[0] = (m_grabVector.vec[0] + mouseX);
        clampWindowSize();
        moveOffset.vec[0] -= m_size.vec[0]*0.5f;
        m_position.vec[0] -= moveOffset.vec[0];
        return true;
    }

    if (m_grabWindow)
    {
        // Move window
        m_position.vec[0] = (m_grabVector.vec[0] + mouseX);
        m_position.vec[1] = (m_grabVector.vec[1] + mouseY);
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
//  Update renderer current cursor                                            //
////////////////////////////////////////////////////////////////////////////////
/*GUICursorType GUIWindow::updateCursor(float mouseX, float mouseY)
{
    if (m_resizable)
    {
        if (m_grabTop && m_grabLeft)
        {
            return GUICURSOR_NWSE;
        }
        if (m_grabTop && m_grabRight)
        {
            return GUICURSOR_NESW;
        }
        if (m_grabBottom && m_grabLeft)
        {
            return GUICURSOR_NESW;
        }
        if (m_grabBottom && m_grabRight)
        {
            return GUICURSOR_NWSE;
        }
        if (m_grabTop)
        {
            return GUICURSOR_NS;
        }
        if (m_grabBottom)
        {
            return GUICURSOR_NS;
        }
        if (m_grabLeft)
        {
            return GUICURSOR_EW;
        }
        if (m_grabRight)
        {
            return GUICURSOR_EW;
        }
        if (m_grabWindow)
        {
            return GUICURSOR_DEFAULT;
        }

        if (isTopResizePicking(mouseX, mouseY) &&
            isLeftResizePicking(mouseX, mouseY))
        {
            return GUICURSOR_NWSE;
        }
        if (isTopResizePicking(mouseX, mouseY) &&
            isRightResizePicking(mouseX, mouseY))
        {
            return GUICURSOR_NESW;
        }
        if (isBottomResizePicking(mouseX, mouseY) &&
            isLeftResizePicking(mouseX, mouseY))
        {
            return GUICURSOR_NESW;
        }
        if (isBottomResizePicking(mouseX, mouseY) &&
            isRightResizePicking(mouseX, mouseY))
        {
            return GUICURSOR_NWSE;
        }
        if (isTopResizePicking(mouseX, mouseY))
        {
            return GUICURSOR_NS;
        }
        if (isBottomResizePicking(mouseX, mouseY))
        {
            return GUICURSOR_NS;
        }
        if (isLeftResizePicking(mouseX, mouseY))
        {
            return GUICURSOR_EW;
        }
        if (isRightResizePicking(mouseX, mouseY))
        {
            return GUICURSOR_EW;
        }
    }

    return GUICURSOR_DEFAULT;
}*/


////////////////////////////////////////////////////////////////////////////////
//  Render window                                                             //
////////////////////////////////////////////////////////////////////////////////
void GUIWindow::render()
{
    // Compute window transformations
    computeTransforms();

    // Upload model matrix
    GRenderer.currentShader->sendModelMatrix(m_matrix);

    // Send uniforms constants
    GRenderer.currentShader->sendColor(m_color);
    GRenderer.currentShader->sendSize(m_size);
    GRenderer.currentShader->sendTime(m_uvFactor);

    // Render sprite
    GRenderer.currentBuffer->render();
}


////////////////////////////////////////////////////////////////////////////////
//  Clamp window size                                                         //
////////////////////////////////////////////////////////////////////////////////
void GUIWindow::clampWindowSize()
{
    // Clamp window width
    if (m_size.vec[0] <= m_minSize.vec[0]) { m_size.vec[0] = m_minSize.vec[0]; }
    if (m_size.vec[0] >= m_maxSize.vec[0]) { m_size.vec[0] = m_maxSize.vec[0]; }

    // Clamp window height
    if (m_size.vec[1] <= m_minSize.vec[1]) { m_size.vec[1] = m_minSize.vec[1]; }
    if (m_size.vec[1] >= m_maxSize.vec[1]) { m_size.vec[1] = m_maxSize.vec[1]; }
}
