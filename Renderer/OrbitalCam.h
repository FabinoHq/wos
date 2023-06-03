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
//     Renderer/OrbitalCam.h : Orbital camera management                      //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_ORBITALCAM_HEADER
#define WOS_RENDERER_ORBITALCAM_HEADER

    #include "../System/System.h"
    #include "../Math/Math.h"
    #include "../Math/Vector3.h"
    #include "../Math/Matrix4x4.h"
    #include "Camera.h"

    #include <cstdint>
    #include <cstring>


    ////////////////////////////////////////////////////////////////////////////
    //  Orbital camera default settings                                       //
    ////////////////////////////////////////////////////////////////////////////
    const float OrbitalCameraMouseFactor = 0.004f;
    const float OrbitalCameraMinAngle = -(Math::PiHalf-0.001f);
    const float OrbitalCameraMaxAngle = (Math::PiHalf-0.001f);
    const float OrbitalCameraMinDistance = 0.0f;
    const float OrbitalCameraMaxDistance = 10.0f;


    ////////////////////////////////////////////////////////////////////////////
    //  OrbitalCam class definition                                           //
    ////////////////////////////////////////////////////////////////////////////
    class OrbitalCam : public Camera
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  OrbitalCam default constructor                                //
            ////////////////////////////////////////////////////////////////////
            OrbitalCam();

            ////////////////////////////////////////////////////////////////////
            //  OrbitalCam virtual destructor                                 //
            ////////////////////////////////////////////////////////////////////
            virtual ~OrbitalCam();


            ////////////////////////////////////////////////////////////////////
            //  Compute orbital camera                                        //
            //  return : True if the orbital camera is successfully computed  //
            ////////////////////////////////////////////////////////////////////
            virtual bool compute(float ratio, float frametime);


            ////////////////////////////////////////////////////////////////////
            //  Set orbital camera target                                     //
            ////////////////////////////////////////////////////////////////////
            void setTarget(const Vector3& target);

            ////////////////////////////////////////////////////////////////////
            //  Set orbital camera target                                     //
            ////////////////////////////////////////////////////////////////////
            void setTarget(float targetX, float targetY, float targetZ);

            ////////////////////////////////////////////////////////////////////
            //  Set orbital camera distance from target                       //
            ////////////////////////////////////////////////////////////////////
            void setDistance(float distance);

            ////////////////////////////////////////////////////////////////////
            //  Set orbital camera speed                                      //
            ////////////////////////////////////////////////////////////////////
            inline void setSpeed(float speed)
            {
                m_speed = speed;
            }

            ////////////////////////////////////////////////////////////////////
            //  Handle mouse move event                                       //
            ////////////////////////////////////////////////////////////////////
            void mouseMove(float mouseDx, float mouseDy);

            ////////////////////////////////////////////////////////////////////
            //  Handle mouse press event                                      //
            ////////////////////////////////////////////////////////////////////
            inline void mousePress()
            {
                m_mousePressed = true;
            }

            ////////////////////////////////////////////////////////////////////
            //  Handle mouse release event                                    //
            ////////////////////////////////////////////////////////////////////
            inline void mouseRelease()
            {
                m_mousePressed = false;
            }

            ////////////////////////////////////////////////////////////////////
            //  Handle mouse wheel event                                      //
            ////////////////////////////////////////////////////////////////////
            void mouseWheel(int mouseWheel);


        private:
            ////////////////////////////////////////////////////////////////////
            //  OrbitalCam private copy constructor : Not copyable            //
            ////////////////////////////////////////////////////////////////////
            OrbitalCam(const OrbitalCam&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  OrbitalCam private copy operator : Not copyable               //
            ////////////////////////////////////////////////////////////////////
            OrbitalCam& operator=(const OrbitalCam&) = delete;


        private:
            Vector3     m_target;       // Orbitalcam target
            float       m_distance;     // Orbitalcam distance from target
            float       m_speed;        // Orbitalcam speed

            bool        m_mousePressed; // Orbitalcam mouse pressed state
            bool        m_forward;      // Orbitalcam forward state
            bool        m_backward;     // Orbitalcam backward state
    };


#endif // WOS_RENDERER_ORBITALCAM_HEADER
