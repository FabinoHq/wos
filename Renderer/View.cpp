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
//     Renderer/View.cpp : View management                                    //
////////////////////////////////////////////////////////////////////////////////
#include "View.h"


////////////////////////////////////////////////////////////////////////////////
//  View default constructor                                                  //
////////////////////////////////////////////////////////////////////////////////
View::View() :
Transform2(),
m_projMatrix(),
m_projViewMatrix()
{
    m_projMatrix.reset();
    m_projViewMatrix.reset();
}

////////////////////////////////////////////////////////////////////////////////
//  View virtual destructor                                                   //
////////////////////////////////////////////////////////////////////////////////
View::~View()
{
    m_projViewMatrix.reset();
    m_projMatrix.reset();
}


////////////////////////////////////////////////////////////////////////////////
//  Init view                                                                 //
//  return : True if the view is successfully created                         //
////////////////////////////////////////////////////////////////////////////////
bool View::init()
{
    // Reset view transformations
    resetTransforms();

    // Set view size
    setSize(1.0f, 1.0f);

    // Reset projection matrix
    m_projMatrix.setOrthographic(-1.0f, 1.0f, 1.0f, -1.0f, -2.0f, 2.0f);
    m_projMatrix.translateZ(-1.0f);

    // Reset projview matrix
    m_projViewMatrix.set(m_projMatrix);
    m_projViewMatrix *= m_matrix;

    // Reset view matrix
    m_matrix.setIdentity();

    // View is successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy view                                                              //
////////////////////////////////////////////////////////////////////////////////
void View::destroyView()
{
    m_projViewMatrix.reset();
    m_projMatrix.reset();
    resetTransforms();
}

////////////////////////////////////////////////////////////////////////////////
//  Compute view                                                              //
//  return : True if the view is successfully computed                        //
////////////////////////////////////////////////////////////////////////////////
bool View::compute(float ratio)
{
    // Compute projection matrix
    m_projMatrix.setOrthographic(-ratio, ratio, 1.0f, -1.0f, -2.0f, 2.0f);
    m_projMatrix.translateZ(-1.0f);

    // Compute view matrix
    m_matrix.setIdentity();
    m_matrix.translate(-m_position);
    m_matrix.rotateZ(-m_angle);
    m_matrix.translate(m_origin);
    m_matrix.scale(m_size);

    // Compute projview matrix
    m_projViewMatrix.set(m_projMatrix);
    m_projViewMatrix *= m_matrix;

    // View successfully computed
    return true;
}
