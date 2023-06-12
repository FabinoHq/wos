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
//     Renderer/Camera.cpp : Camera management                                //
////////////////////////////////////////////////////////////////////////////////
#include "Camera.h"
#include "Renderer.h"


////////////////////////////////////////////////////////////////////////////////
//  Camera default constructor                                                //
////////////////////////////////////////////////////////////////////////////////
Camera::Camera() :
Transform3(),
m_projMatrix(),
m_projViewMatrix(),
m_target(0.0f, 0.0f, 0.0f),
m_upward(0.0f, 0.0f, 0.0f),
m_fovy(0.0f),
m_nearPlane(0.0f),
m_farPlane(0.0f)
{
    m_projMatrix.reset();
    m_projViewMatrix.reset();
}

////////////////////////////////////////////////////////////////////////////////
//  Camera virtual destructor                                                 //
////////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
    m_farPlane = 0.0f;
    m_nearPlane = 0.0f;
    m_fovy = 0.0f;
    m_upward.reset();
    m_target.reset();
    m_projViewMatrix.reset();
    m_projMatrix.reset();
}


////////////////////////////////////////////////////////////////////////////////
//  Init camera                                                               //
//  return : True if the camera is successfully created                       //
////////////////////////////////////////////////////////////////////////////////
bool Camera::init()
{
    // Reset camera transforms
    resetTransforms();

    // Reset camera target vector
    m_target.reset();

    // Reset camera upward vector
    m_upward.set(0.0f, 1.0f, 0.0f);

    // Reset camera fovy
    m_fovy = CameraDefaultFovy;

    // Reset camera Z near and Z far
    m_nearPlane = CameraDefaultNearPlane;
    m_farPlane = CameraDefaultFarPlane;

    // Reset projection matrix
    m_projMatrix.setPerspective(
        m_fovy, GRenderer.getRatio(), m_nearPlane, m_farPlane
    );

    // Reset view matrix
    m_matrix.setIdentity();

    // Reset projview matrix
    m_projViewMatrix.set(m_projMatrix);
    m_projViewMatrix *= m_matrix;

    // Camera is successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy camera                                                            //
////////////////////////////////////////////////////////////////////////////////
void Camera::destroyCamera()
{
    m_farPlane = 0.0f;
    m_nearPlane = 0.0f;
    m_fovy = 0.0f;
    m_upward.reset();
    m_target.reset();
    m_angles.reset();
    m_projViewMatrix.reset();
    m_projMatrix.reset();
    resetTransforms();
}


////////////////////////////////////////////////////////////////////////////////
//  Compute camera                                                            //
//  return : True if the camera is successfully computed                      //
////////////////////////////////////////////////////////////////////////////////
bool Camera::compute(float ratio)
{
    // Compute camera target
    m_target.vec[0] = std::cos(m_angles.vec[0]);
    m_target.vec[0] *= std::sin(m_angles.vec[1]);
    m_target.vec[1] = std::sin(m_angles.vec[0]);
    m_target.vec[2] = std::cos(m_angles.vec[0]);
    m_target.vec[2] *= std::cos(m_angles.vec[1]);
    m_target.normalize();

    // Compute projection matrix
    m_projMatrix.setPerspective(m_fovy, ratio, m_nearPlane, m_farPlane);

    // Compute view matrix
    m_matrix.setIdentity();
    m_matrix.rotate(-m_angles);
    m_matrix.translate(-m_position);

    // Compute projview matrix
    m_projViewMatrix.set(m_projMatrix);
    m_projViewMatrix *= m_matrix;

    // Camera successfully computed
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Compute camera from another camera                                        //
//  return : True if the camera is successfully computed                      //
////////////////////////////////////////////////////////////////////////////////
bool Camera::compute(float ratio, Camera& camera)
{
    // Copy camera parameters
    m_origin = camera.m_origin;
    m_position = camera.m_position;
    m_size = camera.m_size;
    m_angles = camera.m_angles;

    // Compute projection matrix
    m_projMatrix.setPerspective(m_fovy, ratio, m_nearPlane, m_farPlane);

    // Compute view matrix
    m_matrix.set(camera.m_matrix);

    // Compute projview matrix
    m_projViewMatrix.set(m_projMatrix);
    m_projViewMatrix *= m_matrix;

    // Camera successfully computed
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Bind camera                                                               //
////////////////////////////////////////////////////////////////////////////////
void Camera::bind()
{
    GRenderer.currentView = 0;
    GRenderer.currentCamera = this;
    GRenderer.currentShader->sendProjViewMatrix(m_projViewMatrix);
}
