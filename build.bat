:: Activate emsdk
@CALL D:\emsdk\emsdk activate latest

:: Build WOS
@CALL emcc -std=c++17 -O3 -fno-exceptions -fno-rtti -fomit-frame-pointer ^
    -ffunction-sections -fno-trapping-math -fno-math-errno -fno-signed-zeros ^
    -W -Wall -pthread -lGL -s WASM=1 -s USE_PTHREADS=1 -s MAX_WEBGL_VERSION=2 ^
    -s OFFSCREENCANVAS_SUPPORT=1 -s OFFSCREEN_FRAMEBUFFER=1 ^
    -s DYNAMIC_EXECUTION=0 ^
    -o wos.js ^
    --use-preload-plugins --preload-file textures ^
    System/SysMessage.cpp ^
    System/SysCPU.cpp ^
    System/SysClock.cpp ^
    System/SysThread.cpp ^
    System/SysWindow.cpp ^
    Compress/ZLib.cpp ^
    Renderer/Renderer.cpp ^
    Renderer/Shader.cpp ^
    Renderer/VertexBuffer.cpp ^
    Renderer/Texture.cpp ^
    Renderer/View.cpp ^
    Renderer/Sprite.cpp ^
    Renderer/ProcSprite.cpp ^
    Resources/Resources.cpp ^
    Resources/TextureLoader.cpp ^
    Game/Game.cpp ^
    Wos.cpp ^
    main.cpp

:: System pause
PAUSE
