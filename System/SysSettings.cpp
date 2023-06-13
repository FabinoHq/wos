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
//     System/SysSettings.cpp : System Settings management                    //
////////////////////////////////////////////////////////////////////////////////
#include "SysSettings.h"
#include "SysWindow.h"


////////////////////////////////////////////////////////////////////////////////
//  SysSettings global instance                                               //
////////////////////////////////////////////////////////////////////////////////
SysSettings GSysSettings = SysSettings();


////////////////////////////////////////////////////////////////////////////////
//  SysSettings default constructor                                           //
////////////////////////////////////////////////////////////////////////////////
SysSettings::SysSettings() :
m_maxAnisotropicFiltering(ANISOTROPIC_FILTERING_NONE),
m_anisotropicFiltering(ANISOTROPIC_FILTERING_NONE)
{

}

////////////////////////////////////////////////////////////////////////////////
//  SysSettings destructor                                                    //
////////////////////////////////////////////////////////////////////////////////
SysSettings::~SysSettings()
{
    m_anisotropicFiltering = ANISOTROPIC_FILTERING_NONE;
    m_maxAnisotropicFiltering = ANISOTROPIC_FILTERING_NONE;
}


////////////////////////////////////////////////////////////////////////////////
//  Load system settings                                                      //
//  return : True if system settings are successfully loaded                  //
////////////////////////////////////////////////////////////////////////////////
bool SysSettings::loadSettings()
{
    // Temp set settings
    m_anisotropicFiltering = ANISOTROPIC_FILTERING_8X;
    //m_anisotropicFiltering = ANISOTROPIC_FILTERING_NONE;

    // System settings successfully loaded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Adjust system settings according to device properties                     //
////////////////////////////////////////////////////////////////////////////////
void SysSettings::adjustSettings()
{
    // Set max anistrotropic filtering
    m_maxAnisotropicFiltering = ANISOTROPIC_FILTERING_NONE;

    if (GSysWindow.enableExtension("EXT_texture_filter_anisotropic"))
    {
        float maxAnisotropy = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        if (maxAnisotropy >= 2.0f)
        {
            m_maxAnisotropicFiltering = ANISOTROPIC_FILTERING_2X;
        }
        if (maxAnisotropy >= 4.0f)
        {
            m_maxAnisotropicFiltering = ANISOTROPIC_FILTERING_4X;
        }
        if (maxAnisotropy >= 8.0f)
        {
            m_maxAnisotropicFiltering = ANISOTROPIC_FILTERING_8X;
        }
        if (maxAnisotropy >= 16.0f)
        {
            m_maxAnisotropicFiltering = ANISOTROPIC_FILTERING_16X;
        }
    }

    // Adjust anistrotropic filtering
    if (m_anisotropicFiltering >= m_maxAnisotropicFiltering)
    {
        m_anisotropicFiltering = m_maxAnisotropicFiltering;
    }
}
