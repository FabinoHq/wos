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
//  MeshLoader default constructor                                            //
////////////////////////////////////////////////////////////////////////////////
MeshLoader::MeshLoader() :
m_state(MESHLOADER_STATE_NONE),
m_stateMutex(),
m_meshes(0),
m_vertices(0),
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
//  Upload vertex buffer to graphics memory                                   //
//  return : True if vertex buffer is successfully uploaded                   //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::uploadVertexBuffer(VertexBuffer& vertexBuffer,
    const float* vertices, const uint32_t* indices,
    uint32_t verticesCount, uint32_t indicesCount)
{
    // Set current thread as current context
    GSysWindow.setThread();

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
    /*if (!m_meshes[MESHES_DEFAULT].createBuffer(
        DefaultVertices, DefaultIndices,
        DefaultVerticesCount, DefaultIndicesCount,
        VERTEX_INPUTS_DEFAULT))
    {
        // Could not load default vertex buffer
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
    }*/

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
    /*if (!loadVMSH(m_meshes[MESHES_TEST], "Models/testmodel.vmsh"))
    {
        // Could not load test static mesh
        return false;
    }*/

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
//  Load mesh from VMSH file                                                  //
//  return : True if the mesh is successfully loaded                          //
////////////////////////////////////////////////////////////////////////////////
bool MeshLoader::loadVMSH(VertexBuffer& vertexBuffer,
    const std::string& filepath)
{
    // Init vertices and indices count
    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;

    // Load mesh data from file
    std::ifstream file;
    file.open(filepath.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        // Could not load mesh data file
        return false;
    }

    // Read VMSH header
    char header[4] = {0};
    char majorVersion = 0;
    char minorVersion = 0;
    file.read(header, sizeof(char)*4);
    file.read(&majorVersion, sizeof(char));
    file.read(&minorVersion, sizeof(char));

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
    file.read(&type, sizeof(char));
    if (type != 0)
    {
        // Invalid VMSH type
        return false;
    }

    // Read vertices and indices count
    file.read((char*)&verticesCount, sizeof(uint32_t));
    file.read((char*)&indicesCount, sizeof(uint32_t));
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
    file.read((char*)m_vertices, sizeof(float)*verticesCount);

    // Read indices
    file.read((char*)m_indices, sizeof(uint32_t)*indicesCount);

    // Close file
    file.close();

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
