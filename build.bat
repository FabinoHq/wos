:: Activate emsdk
@CALL D:\emsdk\emsdk activate latest

:: Build WOS
@CALL emcc -std=c++17 -O3 -fno-exceptions -fno-rtti -fomit-frame-pointer ^
    -ffunction-sections -fno-trapping-math -fno-math-errno -fno-signed-zeros ^
    -W -Wall -pthread -lGL -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD ^
    -s USE_WEBGL2=1 -s MAX_WEBGL_VERSION=2 -s OFFSCREENCANVAS_SUPPORT=1 ^
    -s OFFSCREEN_FRAMEBUFFER=1 -s OFFSCREENCANVASES_TO_PTHREAD='#woscreen' ^
    -o wos.js ^
    System/SysMessage.cpp ^
    System/SysCPU.cpp ^
    System/SysClock.cpp ^
    System/SysThread.cpp ^
    System/SysWindow.cpp ^
    Renderer/Renderer.cpp ^
    Renderer/Shader.cpp ^
    Renderer/VertexBuffer.cpp ^
    Renderer/View.cpp ^
    Renderer/ProcSprite.cpp ^
    Game/Game.cpp ^
    Wos.cpp ^
    main.cpp

:: System pause
PAUSE
