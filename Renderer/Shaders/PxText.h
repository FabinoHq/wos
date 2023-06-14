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
//     Renderer/Shaders/PxText.h : GUI PixelText fragment shader              //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_RENDERER_SHADERS_PXTEXT_HEADER
#define WOS_RENDERER_SHADERS_PXTEXT_HEADER


    ////////////////////////////////////////////////////////////////////////////
    //  Pixel text fragment shader                                            //
    ////////////////////////////////////////////////////////////////////////////
    const char PxTextFragmentShaderSrc[] =
    "#version 100\n"
    "precision highp float;\n"
    "precision highp int;\n"
    "varying vec2 texCoords;\n"
    "uniform sampler2D texSampler;\n"
    "uniform vec4 constants_color;\n"
    "uniform vec2 constants_offset;\n"
    "uniform vec2 constants_size;\n"
    "uniform float constants_time;\n"
    "\n"
    "// Main shader entry point\n"
    "void main()\n"
    "{\n"
    "    // Compute distance field\n"
    "    float dist = texture2D(\n"
    "        texSampler, (texCoords*constants_size)+constants_offset\n"
    "    ).a;\n"
    "\n"
    "    // Compute output color (constants_time is the smooth amount)\n"
    "    gl_FragColor = vec4(\n"
    "        constants_color.r, constants_color.g, constants_color.b,\n"
    "        smoothstep(0.5-constants_time, 0.5+constants_time, dist)*\n"
    "        constants_color.a\n"
    "    );\n"
    "}\n";


#endif // WOS_RENDERER_SHADERS_PXTEXT_HEADER
