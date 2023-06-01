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
//     Renderer/VertexBuffer.cpp : VertexBuffer management                    //
////////////////////////////////////////////////////////////////////////////////
#include "VertexBuffer.h"
#include "Renderer.h"


////////////////////////////////////////////////////////////////////////////////
//  VertexBuffer default constructor                                          //
////////////////////////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer() :
m_vertexBuffer(0),
m_elementBuffer(0),
m_indicesCount(0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  VertexBuffer destructor                                                   //
////////////////////////////////////////////////////////////////////////////////
VertexBuffer::~VertexBuffer()
{
    // Destroy vertex buffer
    if (m_elementBuffer) { glDeleteBuffers(1, &m_elementBuffer); }
    m_elementBuffer = 0;
    if (m_vertexBuffer) { glDeleteBuffers(1, &m_vertexBuffer); }
    m_vertexBuffer = 0;
}


////////////////////////////////////////////////////////////////////////////////
//  Create Vertex buffer                                                      //
////////////////////////////////////////////////////////////////////////////////
bool VertexBuffer::createBuffer()
{
    // Create vertex buffer
    glGenBuffers(1, &m_vertexBuffer);
    if (!m_vertexBuffer)
    {
        // Unable to create vertex buffer
        return false;
    }

    // Create element buffer
    glGenBuffers(1, &m_elementBuffer);
    if (!m_elementBuffer)
    {
        // Unable to create element buffer
        return false;
    }

    // Update VBO
    if (!updateBuffer(DefaultVertices, DefaultIndices,
        DefaultVerticesCount, DefaultIndicesCount))
    {
        // Unable to update VBO
        return false;
    }

    // Set indices count
    m_indicesCount = DefaultIndicesCount;

    // Vertex buffer is successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Create Vertex buffer                                                      //
////////////////////////////////////////////////////////////////////////////////
bool VertexBuffer::createBuffer(
    const float* vertices, const unsigned int* indices,
    uint32_t verticesCount, uint32_t indicesCount)
{
    // Create vertex buffer
    glGenBuffers(1, &m_vertexBuffer);
    if (!m_vertexBuffer)
    {
        // Unable to create vertex buffer
        return false;
    }

    // Create element buffer
    glGenBuffers(1, &m_elementBuffer);
    if (!m_elementBuffer)
    {
        // Unable to create element buffer
        return false;
    }

    // Update VBO
    if (!updateBuffer(vertices, indices, verticesCount, indicesCount))
    {
        // Unable to update VBO
        return false;
    }

    // Set indices count
    m_indicesCount = indicesCount;

    // Vertex buffer is successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Update Vertex buffer                                                      //
////////////////////////////////////////////////////////////////////////////////
bool VertexBuffer::updateBuffer(
    const float* vertices, const unsigned int* indices,
    uint32_t verticesCount, uint32_t indicesCount)
{
    // Upload data to vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
        verticesCount*sizeof(float), vertices, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Upload indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indicesCount*sizeof(unsigned int), indices, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Vertex buffer is successfully updated
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Render Vertex buffer                                                      //
////////////////////////////////////////////////////////////////////////////////
void VertexBuffer::render()
{
    // Bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);

    // Enable vertices array
    glEnableVertexAttribArray(GRenderer.currentShader->getVerticesLocation());
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0
    );

    // Enable texcoords array
    glEnableVertexAttribArray(GRenderer.currentShader->getTexCoordsLocation());
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float))
    );

    // Render vertex buffer
    glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
}
