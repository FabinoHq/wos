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
//     Renderer/Shaders/Ellipse.h : Ellipse shader                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_SHADERS_ELLIPSE_HEADER
#define WOS_RENDERER_SHADERS_ELLIPSE_HEADER


    ////////////////////////////////////////////////////////////////////////////
    //  Ellipse fragment shader                                               //
    ////////////////////////////////////////////////////////////////////////////
    const char EllipseFragmentShaderSrc[] =
    "#version 100\n"
    "precision highp float;\n"
    "precision highp int;\n"
    "varying vec2 texCoords;\n"
    "uniform vec4 constants_color;\n"
    "uniform float constants_time;\n"
    "\n"
    "// Main shader entry point\n"
    "void main()\n"
    "{\n"
    "    // Compute ellipse shape (constants_time is the smooth amount)\n"
    "    vec4 ellipseShape = vec4(1.0, 1.0, 1.0, smoothstep(\n"
    "        0.499, 0.498-constants_time, length(vec2(0.5, 0.5)-texCoords))\n"
    "    );\n"
    "\n"
    "    // Compute output color\n"
    "    gl_FragColor = (constants_color*ellipseShape);\n"
    "}\n";


#endif // WOS_RENDERER_SHADERS_ELLIPSE_HEADER
