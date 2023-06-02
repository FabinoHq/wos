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
//     Game/Game.h : Main game class management                               //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_GAME_GAME_HEADER
#define WOS_GAME_GAME_HEADER

    #include "../System/System.h"
    #include "../System/SysEvent.h"

    #include "../Renderer/Renderer.h"
    #include "../Renderer/View.h"
    #include "../Renderer/Camera.h"
    #include "../Renderer/Texture.h"
    #include "../Renderer/Sprite.h"
    #include "../Renderer/ProcSprite.h"
    #include "../Renderer/Shapes/RectangleShape.h"
    #include "../Renderer/Shapes/EllipseShape.h"

    #include "../Resources/Resources.h"


    ////////////////////////////////////////////////////////////////////////////
    //  Game main class definition                                            //
    ////////////////////////////////////////////////////////////////////////////
    class Game
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Game default constructor                                      //
            ////////////////////////////////////////////////////////////////////
            Game();

            ////////////////////////////////////////////////////////////////////
            //  Game destructor                                               //
            ////////////////////////////////////////////////////////////////////
            ~Game();


            ////////////////////////////////////////////////////////////////////
            //  Init game                                                     //
            //  return : True if game is ready, false otherwise               //
            ////////////////////////////////////////////////////////////////////
            bool init();


            ////////////////////////////////////////////////////////////////////
            //  Compute game events                                           //
            ////////////////////////////////////////////////////////////////////
            void events(Event& event);

            ////////////////////////////////////////////////////////////////////
            //  Compute game logic                                            //
            ////////////////////////////////////////////////////////////////////
            void compute(float frametime);

            ////////////////////////////////////////////////////////////////////
            //  Render game                                                   //
            ////////////////////////////////////////////////////////////////////
            void render();


        private:
            ////////////////////////////////////////////////////////////////////
            //  Game private copy constructor : Not copyable                  //
            ////////////////////////////////////////////////////////////////////
            Game(const Game&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Game private copy operator : Not copyable                     //
            ////////////////////////////////////////////////////////////////////
            Game& operator=(const Game&) = delete;


        private:
            View            m_view;             // View
            Camera          m_camera;           // Camera

            Sprite          m_sprite;           // Sprite
            ProcSprite      m_procSprite;       // Procedural sprite
            RectangleShape  m_rectangle;        // Rectangle shape
            EllipseShape    m_ellipse;          // Ellipse shape

            float           m_mouseX;           // Mouse X position
            float           m_mouseY;           // Mouse Y position
    };


#endif // WOS_GAME_GAME_HEADER
