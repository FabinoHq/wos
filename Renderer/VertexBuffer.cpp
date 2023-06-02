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
#include "../Resources/Resources.h"


////////////////////////////////////////////////////////////////////////////////
//  VertexBuffer default constructor                                          //
////////////////////////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer() :
vertexType(VERTEX_INPUTS_DEFAULT),
vertexBuffer(0),
elementBuffer(0),
indicesRender(0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  VertexBuffer destructor                                                   //
////////////////////////////////////////////////////////////////////////////////
VertexBuffer::~VertexBuffer()
{
    destroyBuffer();
}


////////////////////////////////////////////////////////////////////////////////
//  Create vertex buffer                                                      //
////////////////////////////////////////////////////////////////////////////////
bool VertexBuffer::createBuffer()
{
    // Upload vertex buffer to graphics memory
    if (!GResources.meshes.createVertexBuffer(*this,
        DefaultVertices, DefaultIndices,
        DefaultVerticesCount, DefaultIndicesCount))
    {
        // Could not upload vertex buffer to graphics memory
        return false;
    }

    // Set indices count
    indicesRender = DefaultIndicesCount;

    // Vertex buffer is successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Create vertex buffer                                                      //
////////////////////////////////////////////////////////////////////////////////
bool VertexBuffer::createBuffer(
    const float* vertices, const unsigned int* indices,
    uint32_t verticesCount, uint32_t indicesCount,
    VertexInputsType vertexInputType)
{
    // Upload vertex buffer to graphics memory
    if (!GResources.meshes.createVertexBuffer(*this,
        vertices, indices, verticesCount, indicesCount))
    {
        // Could not upload vertex buffer to graphics memory
        return false;
    }

    // Set indices count
    indicesRender = indicesCount;

    // Set vertex input type
    vertexType = vertexInputType;

    // Vertex buffer is successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Update vertex buffer                                                      //
////////////////////////////////////////////////////////////////////////////////
bool VertexBuffer::updateBuffer(
    const float* vertices, const unsigned int* indices,
    uint32_t verticesCount, uint32_t indicesCount,
    VertexInputsType vertexInputType)
{
    // Upload vertex buffer to graphics memory
    if (!GResources.meshes.uploadVertexBuffer(*this,
        vertices, indices, verticesCount, indicesCount))
    {
        // Could not upload vertex buffer to graphics memory
        return false;
    }

    // Set indices count
    indicesRender = indicesCount;

    // Set vertex input type
    vertexType = vertexInputType;

    // Vertex buffer is successfully updated
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy vertex buffer                                                     //
////////////////////////////////////////////////////////////////////////////////
void VertexBuffer::destroyBuffer()
{
    // Destroy vertex buffer
    GSysWindow.setThread();
    if (elementBuffer) { glDeleteBuffers(1, &elementBuffer); }
    elementBuffer = 0;
    if (vertexBuffer) { glDeleteBuffers(1, &vertexBuffer); }
    vertexBuffer = 0;
    GSysWindow.releaseThread();
}


////////////////////////////////////////////////////////////////////////////////
//  Render Vertex buffer                                                      //
////////////////////////////////////////////////////////////////////////////////
void VertexBuffer::render()
{
    // Bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    switch (vertexType)
    {
        case VERTEX_INPUTS_STATICMESH:
            // Enable vertices array
            glEnableVertexAttribArray(
                GRenderer.currentShader->getVerticesLocation()
            );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                8*sizeof(float), 0
            );

            // Enable texcoords array
            glEnableVertexAttribArray(
                GRenderer.currentShader->getTexCoordsLocation()
            );
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                8*sizeof(float), (void*)(3*sizeof(float))
            );

            // Enable normals array
            glEnableVertexAttribArray(
                GRenderer.currentShader->getNormalsLocation()
            );
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                8*sizeof(float), (void*)(5*sizeof(float))
            );
            break;

        default:
            // Enable vertices array
            glEnableVertexAttribArray(
                GRenderer.currentShader->getVerticesLocation()
            );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                5*sizeof(float), 0
            );

            // Enable texcoords array
            glEnableVertexAttribArray(
                GRenderer.currentShader->getTexCoordsLocation()
            );
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                5*sizeof(float), (void*)(3*sizeof(float))
            );
            break;
    }

    // Render vertex buffer
    glDrawElements(GL_TRIANGLES, indicesRender, GL_UNSIGNED_INT, 0);
}
