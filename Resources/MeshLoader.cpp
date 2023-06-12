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
//     Resources/MeshLoader.cpp : Mesh loading management                     //
////////////////////////////////////////////////////////////////////////////////
#include "MeshLoader.h"


////////////////////////////////////////////////////////////////////////////////
//  Mesh loaded callback function                                             //
////////////////////////////////////////////////////////////////////////////////
void OnMeshLoaded(void* arg, void* buffer, int size)
{
    // Get callback data
    MeshCallbackData* callbackData = (MeshCallbackData*)arg;
    if (!callbackData) { return; }

    // Allocate data memory for copy
    callbackData->mutex.lock();
    callbackData->data = new (std::nothrow) unsigned char[size];
    if (!callbackData->data)
    {
        // Could not allocate data memory
        callbackData->data = 0;
        callbackData->size = 0;
        callbackData->state = MESHLOADER_CALLBACK_ERROR;
        callbackData->mutex.unlock();
        return;
    }

    // Copy downloaded data into memory
    memcpy(callbackData->data, buffer, size);
    callbackData->size = size;
    callbackData->state = MESHLOADER_CALLBACK_LOADED;
    callbackData->mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
//  Mesh error callback function                                              //
////////////////////////////////////////////////////////////////////////////////
void OnMeshError(void* arg)
{
    // Get callback data
    MeshCallbackData* callbackData = (MeshCallbackData*)arg;
    if (!callbackData) { return; }

    // Set callback data error
    callbackData->mutex.lock();
    callbackData->data = 0;
    callbackData->size = 0;
    callbackData->state = MESHLOADER_CALLBACK_ERROR;
    callbackData->mutex.unlock();
}


////////////////////////////////////////////////////////////////////////////////
//  MeshLoader default constructor                                            //
////////////////////////////////////////////////////////////////////////////////
MeshLoader::MeshLoader() :
m_state(MESHLOADER_STATE_NONE),
m_stateMutex(),
m_meshes(0),
m_vertices(0),
m_indices16(0),
m_indices(0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  MeshLoader virtual destructor                                             //
////////////////////////////////////////////////////////////////////////////////
MeshLoader::~MeshLoader()
{
    if (m_indices) { delete[] m_indices; }
    m_indices = 0;
    if (m_indices16) { delete[] m_indices16; }
    m_indices16 = 0;
    if (m_vertices) { delete[] m_vertices; }
    m_vertices = 0;
    if (m_meshes) { delete[] m_meshes; }
    m_meshes = 0;
    m_state = MESHLOADER_STATE_NONE;
}


////////////////////////////////////////////////////////////////////////////////
//  MeshLoader thread process                                                 //
////////////////////////////////////////////////////////////////////////////////
void MeshLoader::process()
{
    MeshLoaderState state = MESHLOADER_STATE_NONE;
    m_stateMutex.lock();
    state = m_state;
    m_stateMutex.unlock();

    switch (state)
    {
        case MESHLOADER_STATE_NONE:
            // Boot to init state
            m_stateMutex.lock();
            m_state = MESHLOADER_STATE_INIT;
            m_stateMutex.unlock();
            break;

        case MESHLOADER_STATE_INIT:
            // Init mesh loader
            if (init())
            {
                m_stateMutex.lock();
                m_state = MESHLOADER_STATE_LOADEMBEDDED;
                m_stateMutex.unlock();
            }
            else
            {
                m_stateMutex.lock();
                m_state = MESHLOADER_STATE_ERROR;
                m_stateMutex.unlock();
            }
            break;

        case MESHLOADER_STATE_LOADEMBEDDED:
            // Load embedded meshes
            if (loadEmbeddedMeshes())
            {
                m_stateMutex.lock();
                m_state = MESHLOADER_STATE_IDLE;
                m_stateMutex.unlock();
            }
            else
            {
                m_stateMutex.lock();
                m_state = MESHLOADER_STATE_ERROR;
                m_stateMutex.unlock();
            }
            break;

        case MESHLOADER_STATE_IDLE:
            // Mesh loader in idle state
            SysSleep(MeshLoaderIdleSleepTime);
            break;

        case MESHLOADER_STATE_PRELOAD:
            // Preload meshes assets
            if (preloadMeshes())
            {
                m_stateMutex.lock();
                m_state = MESHLOADER_STATE_IDLE;
                m_stateMutex.unlock();
            }
            else
            {
                m_stateMutex.lock();
                m_state = MESHLOADER_STATE_ERROR;
                m_stateMutex.unlock();
            }
            break;

        case MESHLOADER_STATE_LOAD:
            // Load meshes assets
            if (loadMeshes())
            {
                m_stateMutex.lock();
                m_state = MESHLOADER_STATE_IDLE;
                m_stateMutex.unlock();
            }
            else
            {
                m_stateMutex.lock();
                m_state = MESHLOADER_STATE_ERROR;
                m_stateMutex.unlock();
            }
            break;

        case MESHLOADER_STATE_ERROR:
            // Mesh loader error
            SysSleep(MeshLoaderErrorSleepTime);
            break;

        default:
            // Invalid state
            m_stateMutex.lock();
            m_state = MESHLOADER_STATE_ERROR;
            m_stateMutex.unlock();
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////
//  Init MeshLoader                                                           //
//  return : True if mesh loader is ready                                     //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::init()
{
    // Allocate meshes vertex buffers
    m_meshes = new (std::nothrow) VertexBuffer[MESHES_ASSETSCOUNT];
    if (!m_meshes)
    {
        // Could not allocate meshes vertex buffers
        return false;
    }

    // Allocate mesh vertices
    m_vertices = new (std::nothrow) float[MeshLoaderMaxVerticesCount];
    if (!m_vertices)
    {
        // Could not allocate mesh vertices
        return false;
    }

    // Allocate mesh 16bits indices
    m_indices16 = new (std::nothrow) uint16_t[MeshLoaderMaxIndicesCount];
    if (!m_indices16)
    {
        // Could not allocate mesh 16bits indices
        return false;
    }

    // Allocate mesh indices
    m_indices = new (std::nothrow) uint32_t[MeshLoaderMaxIndicesCount];
    if (!m_indices)
    {
        // Could not allocate mesh indices
        return false;
    }

    // Mesh loader ready
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Start preloading meshes assets                                            //
//  return : True if meshes assets are preloading                             //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::startPreload()
{
    bool preLoading = false;
    m_stateMutex.lock();

    // Check mesh loader state
    if (m_state == MESHLOADER_STATE_IDLE)
    {
        // Switch to preload state
        m_state = MESHLOADER_STATE_PRELOAD;
        preLoading = true;
    }

    m_stateMutex.unlock();
    return preLoading;
}

////////////////////////////////////////////////////////////////////////////////
//  Start loading meshes assets                                               //
//  return : True if meshes assets are loading                                //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::startLoading()
{
    bool loading = false;
    m_stateMutex.lock();

    // Check mesh loader state
    if (m_state == MESHLOADER_STATE_IDLE)
    {
        // Switch to load state
        m_state = MESHLOADER_STATE_LOAD;
        loading = true;
    }

    m_stateMutex.unlock();
    return loading;
}

////////////////////////////////////////////////////////////////////////////////
//  Get mesh loader state                                                     //
//  return : Current mesh loader state                                        //
////////////////////////////////////////////////////////////////////////////////
MeshLoaderState MeshLoader::getState()
{
    MeshLoaderState state = MESHLOADER_STATE_NONE;
    m_stateMutex.lock();
    state = m_state;
    m_stateMutex.unlock();
    return state;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy mesh loader                                                       //
////////////////////////////////////////////////////////////////////////////////
void MeshLoader::destroyMeshLoader()
{
    // Delete indices and vertices
    if (m_indices) { delete[] m_indices; }
    m_indices = 0;
    if (m_indices16) { delete[] m_indices16; }
    m_indices16 = 0;
    if (m_vertices) { delete[] m_vertices; }
    m_vertices = 0;

    // Destroy meshes vertex buffers
    for (int i = 0; i < MESHES_ASSETSCOUNT; ++i)
    {
        m_meshes[i].destroyBuffer();
    }
    if (m_meshes) { delete[] m_meshes; }
    m_meshes = 0;
}


////////////////////////////////////////////////////////////////////////////////
//  Load mesh asynchronously and wait for callback                            //
//  return : True if mesh is loaded, false otherwise                          //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::loadMeshAsync(VertexBuffer& vertexBuffer, const char* path)
{
    // Reset callback data
    MeshCallbackData callbackData;
    callbackData.mutex.lock();
    callbackData.state = MESHLOADER_CALLBACK_NONE;
    callbackData.data = 0;
    callbackData.size = 0;
    callbackData.mutex.unlock();

    // Download mesh asynchronously
    emscripten_async_wget_data(
        path, (void*)&callbackData, OnMeshLoaded, OnMeshError
    );
    MeshCallbackState state = MESHLOADER_CALLBACK_NONE;

    // Wait for the mesh to be downloaded
    while (state == MESHLOADER_CALLBACK_NONE)
    {
        callbackData.mutex.lock();
        state = callbackData.state;
        callbackData.mutex.unlock();
        SysSleep(MeshLoaderWaitAsyncSleepTime);
    }
    if ((state == MESHLOADER_CALLBACK_ERROR) || (!callbackData.data))
    {
        // Could not download mesh
        return false;
    }

    // Load mesh from data buffer
    if (!loadVMSH(vertexBuffer, callbackData.data, callbackData.size))
    {
        // Could not load VMSH
        return false;
    }

    // Mesh is successfully loaded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Create and upload vertex buffer to graphics memory                        //
//  return : True if vertex buffer is successfully uploaded                   //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::createVertexBuffer(VertexBuffer& vertexBuffer,
    const float* vertices, const uint32_t* indices,
    uint32_t verticesCount, uint32_t indicesCount)
{
    // Set current thread as current context
    GSysWindow.setThread();

    // Create vertex buffer
    glGenBuffers(1, &vertexBuffer.vertexBuffer);
    if (!vertexBuffer.vertexBuffer)
    {
        // Unable to create vertex buffer
        return false;
    }

    // Create element buffer
    glGenBuffers(1, &vertexBuffer.elementBuffer);
    if (!vertexBuffer.elementBuffer)
    {
        // Unable to create element buffer
        return false;
    }

    // Upload data to vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
        verticesCount*sizeof(float), vertices, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Upload indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer.elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indicesCount*sizeof(unsigned int), indices, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Vertex buffer successfully uploaded
    GSysWindow.releaseThread();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Upload vertex buffer to graphics memory                                   //
//  return : True if vertex buffer is successfully uploaded                   //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::uploadVertexBuffer(VertexBuffer& vertexBuffer,
    const float* vertices, const uint32_t* indices,
    uint32_t verticesCount, uint32_t indicesCount)
{
    // Set current thread as current context
    GSysWindow.setThread();

    // Upload data to vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
        verticesCount*sizeof(float), vertices, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Upload indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer.elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indicesCount*sizeof(unsigned int), indices, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Vertex buffer successfully uploaded
    GSysWindow.releaseThread();
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Load embedded meshes                                                      //
//  return : True if embedded meshes are successfully loaded                  //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::loadEmbeddedMeshes()
{
    // Load default vertex buffer
    if (!m_meshes[MESHES_DEFAULT].createBuffer(
        DefaultVertices, DefaultIndices,
        DefaultVerticesCount, DefaultIndicesCount,
        VERTEX_INPUTS_DEFAULT))
    {
        // Could not load default vertex buffer
        return false;
    }

    // Load plane vertex buffer
    if (!m_meshes[MESHES_PLANE].createBuffer(
        PlaneVertices, PlaneIndices,
        PlaneVerticesCount, PlaneIndicesCount,
        VERTEX_INPUTS_STATICMESH))
    {
        // Could not load plane vertex buffer
        return false;
    }

    // Load cuboid vertex buffer
    if (!m_meshes[MESHES_CUBOID].createBuffer(
        CuboidShapeVertices, CuboidShapeIndices,
        CuboidShapeVerticesCount, CuboidShapeIndicesCount,
        VERTEX_INPUTS_STATICMESH))
    {
        // Could not load cuboid vertex buffer
        return false;
    }

    // Embedded meshes successfully loaded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Preload meshes assets                                                     //
//  return : True if meshes assets are preloaded                              //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::preloadMeshes()
{
    // Load test static mesh
    if (!loadMeshAsync(m_meshes[MESHES_TEST], "models/testmodel.vmsh"))
    {
        // Could not load test static mesh
        return false;
    }

    // Meshes assets are successfully preloaded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Load meshes assets                                                        //
//  return : True if meshes assets are loaded                                 //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::loadMeshes()
{
    // Meshes assets are successfully loaded
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Load mesh from VMSH data buffer                                           //
//  return : True if the mesh is successfully loaded                          //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::loadVMSH(VertexBuffer& vertexBuffer,
    unsigned char* data, int size)
{
    // Init vertices and indices count
    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;
    unsigned char* end = data+size;

    // Read VMSH header
    char header[4] = {0};
    char majorVersion = 0;
    char minorVersion = 0;
    if (data > (end - sizeof(char)*6)) { return false; }
    memcpy(header, data, sizeof(char)*4);
    data += sizeof(char)*4;
    memcpy(&majorVersion, data, sizeof(char));
    data += sizeof(char);
    memcpy(&minorVersion, data, sizeof(char));
    data += sizeof(char);

    // Check VMSH header
    if ((header[0] != 'V') || (header[1] != 'M') ||
        (header[2] != 'S') || (header[3] != 'H'))
    {
        // Invalid VMSH header
        return false;
    }

    // Check VMSH version
    if ((majorVersion != 1) || (minorVersion != 0))
    {
        // Invalid VMSH header
        return false;
    }

    // Read VMSH file type
    char type = 0;
    if (data > (end - sizeof(char))) { return false; }
    memcpy(&type, data, sizeof(char));
    data += sizeof(char);
    if (type != 0)
    {
        // Invalid VMSH type
        return false;
    }

    // Read vertices and indices count
    if (data > (end - sizeof(uint32_t)*2)) { return false; }
    memcpy(&verticesCount, data, sizeof(uint32_t));
    data += sizeof(uint32_t);
    memcpy(&indicesCount, data, sizeof(uint32_t));
    data += sizeof(uint32_t);
    if ((verticesCount <= 0) || (indicesCount <= 0))
    {
        // Invalid vertices or indices count
        return false;
    }

    // Check vertices count
    if (verticesCount >= MeshLoaderMaxVerticesCount)
    {
        // Invalid vertices count
        return false;
    }

    // Check indices count
    if (indicesCount >= MeshLoaderMaxIndicesCount)
    {
        // Invalid indices count
        return false;
    }

    // Read vertices
    if (data > (end - sizeof(float)*verticesCount)) { return false; }
    memcpy((char*)m_vertices, data, sizeof(float)*verticesCount);
    data += sizeof(float)*verticesCount;

    // Read indices
    if (data > (end - sizeof(uint16_t)*indicesCount)) { return false; }
    memcpy((char*)m_indices16, data, sizeof(uint16_t)*indicesCount);
    data += sizeof(uint16_t)*indicesCount;

    // Convert 16bits indices into 32bits indices
    for (uint32_t i = 0; i < indicesCount; ++i)
    {
        m_indices[i] = m_indices16[i];
    }

    // Create vertex buffer
    if (!vertexBuffer.createBuffer(
        m_vertices, m_indices, verticesCount, indicesCount,
        VERTEX_INPUTS_STATICMESH))
    {
        // Could not create vertex buffer
        return false;
    }

    // Mesh successfully loaded
    return true;
}
