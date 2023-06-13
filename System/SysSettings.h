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
//     System/SysSettings.h : System Settings management                      //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_SYSTEM_SYSSETTINGS_HEADER
#define WOS_SYSTEM_SYSSETTINGS_HEADER

    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #include "System.h"


    ////////////////////////////////////////////////////////////////////////////
    //  Anisotropic filtering mode enumeration                                //
    ////////////////////////////////////////////////////////////////////////////
    enum AnisotropicFilteringMode
    {
        ANISOTROPIC_FILTERING_NONE = 0,
        ANISOTROPIC_FILTERING_2X = 1,
        ANISOTROPIC_FILTERING_4X = 2,
        ANISOTROPIC_FILTERING_8X = 3,
        ANISOTROPIC_FILTERING_16X = 4
    };


    ////////////////////////////////////////////////////////////////////////////
    //  SysSettings class definition                                          //
    ////////////////////////////////////////////////////////////////////////////
    class SysSettings
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  SysSettings default constructor                               //
            ////////////////////////////////////////////////////////////////////
            SysSettings();

            ////////////////////////////////////////////////////////////////////
            //  SysSettings destructor                                        //
            ////////////////////////////////////////////////////////////////////
            ~SysSettings();


            ////////////////////////////////////////////////////////////////////
            //  Load system settings                                          //
            //  return : True if system settings are successfully loaded      //
            ////////////////////////////////////////////////////////////////////
            bool loadSettings();

            ////////////////////////////////////////////////////////////////////
            //  Adjust system settings according to device properties         //
            ////////////////////////////////////////////////////////////////////
            void adjustSettings();


            ////////////////////////////////////////////////////////////////////
            //  Set Anisotropic filtering mode                                //
            ////////////////////////////////////////////////////////////////////
            inline void setAnisotropicFilteringMode(
                AnisotropicFilteringMode anisotropicFiltering)
            {
                m_anisotropicFiltering = anisotropicFiltering;
            }


            ////////////////////////////////////////////////////////////////////
            //  Get max anisotropic filtering mode                            //
            ////////////////////////////////////////////////////////////////////
            inline AnisotropicFilteringMode getMaxAnisotropicFilteringMode()
            {
                return m_maxAnisotropicFiltering;
            }


            ////////////////////////////////////////////////////////////////////
            //  Get Anisotropic filtering mode                                //
            ////////////////////////////////////////////////////////////////////
            inline AnisotropicFilteringMode getAnisotropicFilteringMode()
            {
                return m_anisotropicFiltering;
            }


        private:
            ////////////////////////////////////////////////////////////////////
            //  SysSettings private copy constructor : Not copyable           //
            ////////////////////////////////////////////////////////////////////
            SysSettings(const SysSettings&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  SysSettings private copy operator : Not copyable              //
            ////////////////////////////////////////////////////////////////////
            SysSettings& operator=(const SysSettings&) = delete;


        private:
            AnisotropicFilteringMode    m_maxAnisotropicFiltering;

            AnisotropicFilteringMode    m_anisotropicFiltering;
    };


    ////////////////////////////////////////////////////////////////////////////
    //  SysSettings global instance                                           //
    ////////////////////////////////////////////////////////////////////////////
    extern SysSettings GSysSettings;


#endif // WOS_SYSTEM_SYSSETTINGS_HEADER
