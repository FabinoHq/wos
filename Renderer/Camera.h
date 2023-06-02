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
//     Renderer/Camera.h : Camera management                                  //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_CAMERA_HEADER
#define WOS_RENDERER_CAMERA_HEADER

    #include "../System/System.h"
    #include "../Math/Math.h"
    #include "../Math/Vector3.h"
    #include "../Math/Matrix4x4.h"
    #include "../Math/Transform3.h"

    #include <cstdint>
    #include <cstring>


    ////////////////////////////////////////////////////////////////////////////
    //  Camera default settings                                               //
    ////////////////////////////////////////////////////////////////////////////
    const float CameraDefaultFovy = Math::PiHalf;
    const float CameraDefaultNearPlane = 0.1f;
    const float CameraDefaultFarPlane = 1500.0f;
    const float CameraDistanceNearPlane = 200.0f;
    const float CameraDistanceFarPlane = 20000.0f;


    ////////////////////////////////////////////////////////////////////////////
    //  Camera class definition                                               //
    ////////////////////////////////////////////////////////////////////////////
    class Camera : public Transform3
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Camera default constructor                                    //
            ////////////////////////////////////////////////////////////////////
            Camera();

            ////////////////////////////////////////////////////////////////////
            //  Camera virtual destructor                                     //
            ////////////////////////////////////////////////////////////////////
            virtual ~Camera();


            ////////////////////////////////////////////////////////////////////
            //  Init camera                                                   //
            //  return : True if the camera is successfully created           //
            ////////////////////////////////////////////////////////////////////
            bool init();

            ////////////////////////////////////////////////////////////////////
            //  Destroy camera                                                //
            ////////////////////////////////////////////////////////////////////
            void destroyCamera();


            ////////////////////////////////////////////////////////////////////
            //  Compute camera                                                //
            //  return : True if the camera is successfully computed          //
            ////////////////////////////////////////////////////////////////////
            virtual bool compute(float ratio);

            ////////////////////////////////////////////////////////////////////
            //  Compute camera from another camera                            //
            //  return : True if the camera is successfully computed          //
            ////////////////////////////////////////////////////////////////////
            virtual bool compute(float ratio, Camera& camera);

            ////////////////////////////////////////////////////////////////////
            //  Bind camera                                                   //
            ////////////////////////////////////////////////////////////////////
            void bind();


            ////////////////////////////////////////////////////////////////////
            //  Set camera target vector                                      //
            ////////////////////////////////////////////////////////////////////
            inline void setTarget(Vector3& target)
            {
                m_target.vec[0] = target.vec[0];
                m_target.vec[1] = target.vec[1];
                m_target.vec[2] = target.vec[2];
            }

            ////////////////////////////////////////////////////////////////////
            //  Set camera fovy angle                                         //
            ////////////////////////////////////////////////////////////////////
            inline void setFovy(float fovy)
            {
                m_fovy = fovy;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set camera near plane                                         //
            ////////////////////////////////////////////////////////////////////
            inline void setNearPlane(float nearPlane)
            {
                m_nearPlane = nearPlane;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set camera far plane                                          //
            ////////////////////////////////////////////////////////////////////
            inline void setFarPlane(float farPlane)
            {
                m_farPlane = farPlane;
            }


            ////////////////////////////////////////////////////////////////////
            //  Get camera target                                             //
            ////////////////////////////////////////////////////////////////////
            inline Vector3 getTarget() const
            {
                return m_target;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get camera fovy angle                                         //
            ////////////////////////////////////////////////////////////////////
            inline float getFovy() const
            {
                return m_fovy;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get camera near plane                                         //
            ////////////////////////////////////////////////////////////////////
            inline float getNearPlane() const
            {
                return m_nearPlane;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get camera far plane                                          //
            ////////////////////////////////////////////////////////////////////
            inline float getFarPlane() const
            {
                return m_farPlane;
            }


        private:
            ////////////////////////////////////////////////////////////////////
            //  Camera private copy constructor : Not copyable                //
            ////////////////////////////////////////////////////////////////////
            Camera(const Camera&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Camera private copy operator : Not copyable                   //
            ////////////////////////////////////////////////////////////////////
            Camera& operator=(const Camera&) = delete;


        protected:
            Matrix4x4           m_projMatrix;       // Projection matrix
            Matrix4x4           m_projViewMatrix;   // Projview matrix

            Vector3             m_target;           // Camera target vector
            Vector3             m_upward;           // Camera upward vector
            float               m_fovy;             // Camera fovy
            float               m_nearPlane;        // Camera near plane
            float               m_farPlane;         // Camera far plane
    };


#endif // WOS_RENDERER_CAMERA_HEADER
