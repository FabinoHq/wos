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
//   For more information, please refer to <http://unlicense.org>             //
////////////////////////////////////////////////////////////////////////////////
//    WOS : Web Operating System                                              //
//      renderer/nineboxshader.js : Ninebox shader                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  Ninebox fragment shader                                                   //
////////////////////////////////////////////////////////////////////////////////
const nineboxFragmentShaderSrc = [
    "precision mediump float;",
    "precision mediump int;",
    "uniform sampler2D texture;",
    "varying vec2 texCoord;",
    "uniform float alpha;",
    "uniform vec2 uvSize;",
    "uniform float uvFactor;",
    "",
    "void main()",
    "{",
    "    vec2 nineSize = abs(uvSize*uvFactor);",
    "    vec2 curCoord = texCoord*nineSize;",
    "    if (curCoord.x >= 0.25)",
    "    {",
    "        if (curCoord.x >= (nineSize.x-0.25))",
    "        {",
    "            curCoord.x = curCoord.x-nineSize.x;",
    "        }",
    "        else",
    "        {",
    "            curCoord.x = 0.25+mod(curCoord.x, 0.5);",
    "        }",
    "    }",
    "    if (curCoord.y >= 0.25)",
    "    {",
    "        if (curCoord.y >= (nineSize.y-0.25))",
    "        {",
    "            curCoord.y = curCoord.y-nineSize.y;",
    "        }",
    "        else",
    "        {",
    "            curCoord.y = 0.25+mod(curCoord.y, 0.5);",
    "        }",
    "    }",
    "    if (nineSize.x <= 0.5) { curCoord.x = texCoord.x; }",
    "    if (nineSize.y <= 0.5) { curCoord.y = texCoord.y; }",
    "    vec4 texColor = texture2D(texture, curCoord);",
    "    gl_FragColor = vec4(texColor.rgb, texColor.a*alpha);",
    "}",
    ""
].join("\n");
