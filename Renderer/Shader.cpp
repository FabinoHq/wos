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
//     Renderer/Shader.cpp : Shader management                                //
////////////////////////////////////////////////////////////////////////////////
#include "Shader.h"
#include "Renderer.h"


////////////////////////////////////////////////////////////////////////////////
//  Shader default constructor                                                //
////////////////////////////////////////////////////////////////////////////////
Shader::Shader() :
m_shader(-1),
m_verticesLoc(-1),
m_texCoordsLoc(-1),
m_projViewMatrixLoc(-1),
m_modelMatrixLoc(-1)
{

}

////////////////////////////////////////////////////////////////////////////////
//  Shader destructor                                                         //
////////////////////////////////////////////////////////////////////////////////
Shader::~Shader()
{
	// Destroy shader
	destroyShader();
}


////////////////////////////////////////////////////////////////////////////////
//  Create shader                                                             //
////////////////////////////////////////////////////////////////////////////////
bool Shader::createShader(const char* vertexShaderSrc,
	const char* fragmentShaderSrc)
{
	// Check shader sources
	if (!vertexShaderSrc || !fragmentShaderSrc)
	{
		// Invalid shader source
		SysMessage::box() << "[0x3005] Invalid shader source\n";
        SysMessage::box() << "Please update your graphics drivers";
		return false;
	}

	// Create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (!vertexShader)
	{
		// Unable to create vertex shader
		SysMessage::box() << "[0x3006] Unable to create vertex shader\n";
        SysMessage::box() << "Please update your graphics drivers";
		return false;
	}

	// Compile vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSrc, 0);
	glCompileShader(vertexShader);

	int compiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
		//std::cout << infoLog << '\n';

		// Unable to compile vertex shader
		SysMessage::box() << "[0x3007] Unable to compile vertex shader\n";
        SysMessage::box() << "Please update your graphics drivers";
		return false;
	}

	// Create fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragmentShader)
	{
		// Unable to create fragment shader
		SysMessage::box() << "[0x3008] Unable to create fragment shader\n";
        SysMessage::box() << "Please update your graphics drivers";
		return false;
	}

	// Compile fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, 0);
	glCompileShader(fragmentShader);

	compiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
		//std::cout << infoLog << '\n';

		// Unable to compile fragment shader
		SysMessage::box() << "[0x3009] Unable to compile fragment shader\n";
        SysMessage::box() << "Please update your graphics drivers";
		return false;
	}

	// Create shader program
	m_shader = glCreateProgram();
	if (!m_shader)
	{
		// Unable to create shader program
		SysMessage::box() << "[0x300A] Unable to create shader program\n";
        SysMessage::box() << "Please update your graphics drivers";
		return false;
	}
	glAttachShader(m_shader, vertexShader);
	glAttachShader(m_shader, fragmentShader);

	// Bind vertex and texcoords attributes
	glBindAttribLocation(m_shader, 0, "vertexPos");
	glBindAttribLocation(m_shader, 1, "vertexCoords");

	// Link shader
	int linked = 0;
	glLinkProgram(m_shader);
	glGetProgramiv(m_shader, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		// Unable to link shader program
		SysMessage::box() << "[0x300B] Unable to link shader program\n";
        SysMessage::box() << "Please update your graphics drivers";
		return false;
	}

	// Bind shader
	glUseProgram(m_shader);

	// Get shader attributes locations
	m_verticesLoc = glGetAttribLocation(m_shader, "vertexPos");
	if (m_verticesLoc < 0) { return false; }
	m_texCoordsLoc = glGetAttribLocation(m_shader, "vertexCoords");
	if (m_texCoordsLoc < 0) { return false; }

	// Get shader uniforms locations
	m_projViewMatrixLoc = glGetUniformLocation(m_shader, "projViewMatrix");
	if (m_projViewMatrixLoc < 0) { return false; }
	m_modelMatrixLoc = glGetUniformLocation(m_shader, "modelMatrix");
	if (m_modelMatrixLoc < 0) { return false; }

	// Set default identity matrices
	float mat[16];
	mat[0] = 1.0f;
    mat[1] = 0.0f;
    mat[2] = 0.0f;
    mat[3] = 0.0f;
    mat[4] = 0.0f;
    mat[5] = 1.0f;
    mat[6] = 0.0f;
    mat[7] = 0.0f;
    mat[8] = 0.0f;
    mat[9] = 0.0f;
    mat[10] = 1.0f;
    mat[11] = 0.0f;
    mat[12] = 0.0f;
    mat[13] = 0.0f;
    mat[14] = 0.0f;
    mat[15] = 1.0f;
    glUniformMatrix4fv(m_projViewMatrixLoc, 1, GL_FALSE, mat);
    glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, mat);
    glUseProgram(0);

    // Delete sub shader programs
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Shader successfully created
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy shader                                                            //
////////////////////////////////////////////////////////////////////////////////
void Shader::destroyShader()
{
	glUseProgram(0);
	if (m_shader) { glDeleteProgram(m_shader); }
	m_shader = 0;
}
