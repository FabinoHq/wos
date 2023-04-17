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
//     Resources/Resources.cpp : Resources management                         //
////////////////////////////////////////////////////////////////////////////////
#include "Resources.h"


////////////////////////////////////////////////////////////////////////////////
//  Resources global instance                                                 //
////////////////////////////////////////////////////////////////////////////////
Resources GResources = Resources();


////////////////////////////////////////////////////////////////////////////////
//  Resources default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
Resources::Resources() :
textures()
{

}

////////////////////////////////////////////////////////////////////////////////
//  Resources destructor                                                      //
////////////////////////////////////////////////////////////////////////////////
Resources::~Resources()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Init resources loaders                                                    //
//  return : True if resources loaders starting                               //
////////////////////////////////////////////////////////////////////////////////
bool Resources::init()
{
    // Start texture loader thread
    textures.start();

    // Resources loaders are starting
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Preload resources assets                                                  //
//  return : True if resources assets are preloading                          //
////////////////////////////////////////////////////////////////////////////////
bool Resources::preload()
{
    // Start texture preloading
    textures.startPreload();

    // Resources assets are preloading
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Start loading resources assets                                            //
//  return : True if resources assets are loading                             //
////////////////////////////////////////////////////////////////////////////////
bool Resources::startLoading()
{
    // Start textures assets loading
    if (!textures.startLoading())
    {
        // Could not start textures loading
        SysMessage::box() << "[0x4002] Could not start textures loader\n";
        SysMessage::box() << "Please check your resources files";
        return false;
    }

    // Resources assets are loading
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Get resources init status                                                 //
//  return : True if resource loader is ready, false otherwise                //
////////////////////////////////////////////////////////////////////////////////
bool Resources::isInitDone()
{
    // Get resources loader states
    TextureLoaderState textureState = textures.getState();

    // Check resources loader states
    if (textureState == TEXTURELOADER_STATE_IDLE)
    {
        // Resources loaders are ready
        return true;
    }

    // Resources assets are not ready, or an error occured
    return false;
}

////////////////////////////////////////////////////////////////////////////////
//  Get resources loading status                                              //
//  return : True if resources assets are loaded, false otherwise             //
////////////////////////////////////////////////////////////////////////////////
bool Resources::isLoadingDone()
{
    // Get resources loader states
    TextureLoaderState textureState = textures.getState();

    // Check resources loader states
    if (textureState == TEXTURELOADER_STATE_IDLE)
    {
        // Resources assets are loaded
        return true;
    }

    // Resources assets are still loading, or an error occured
    return false;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy resources                                                         //
////////////////////////////////////////////////////////////////////////////////
void Resources::destroyResources()
{
    // Stop texture loader thread
    textures.stop();

    // Destroy texture loader
    textures.destroyTextureLoader();
}
