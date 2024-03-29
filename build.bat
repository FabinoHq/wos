:: Activate emsdk
@CALL D:\emsdk\emsdk activate latest

:: Build WOS
@CALL emcc -std=c++17 -O3 -fno-exceptions -fno-rtti -fomit-frame-pointer ^
    -ffunction-sections -fno-trapping-math -fno-math-errno -fno-signed-zeros ^
    -W -Wall -pthread -lGL -s WASM=1 -s USE_PTHREADS=1 -s MAX_WEBGL_VERSION=2 ^
    -s OFFSCREENCANVAS_SUPPORT=1 -s OFFSCREEN_FRAMEBUFFER=1 ^
    -s DYNAMIC_EXECUTION=0 ^
    -o wos.js ^
    System/SysMessage.cpp ^
    System/SysCPU.cpp ^
    System/SysClock.cpp ^
    System/SysThread.cpp ^
    System/SysWindow.cpp ^
    System/SysMouse.cpp ^
    System/SysSettings.cpp ^
    Compress/ZLib.cpp ^
    Images/PNGFile.cpp ^
    Renderer/Renderer.cpp ^
    Renderer/Shader.cpp ^
    Renderer/VertexBuffer.cpp ^
    Renderer/Texture.cpp ^
    Renderer/View.cpp ^
    Renderer/Camera.cpp ^
    Renderer/FreeFlyCam.cpp ^
    Renderer/OrbitalCam.cpp ^
    Renderer/Sprite.cpp ^
    Renderer/ProcSprite.cpp ^
    Renderer/Plane.cpp ^
    Renderer/StaticMesh.cpp ^
    Renderer/Shapes/RectangleShape.cpp ^
    Renderer/Shapes/EllipseShape.cpp ^
    Renderer/Shapes/CuboidShape.cpp ^
    Renderer/GUI/GUICursor.cpp ^
    Renderer/GUI/GUIPxText.cpp ^
    Renderer/GUI/GUIWindow.cpp ^
    Resources/Resources.cpp ^
    Resources/TextureLoader.cpp ^
    Resources/MeshLoader.cpp ^
    Game/Game.cpp ^
    Wos.cpp ^
    main.cpp

:: System pause
PAUSE
