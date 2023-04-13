:: Activate emsdk
@CALL D:\emsdk\emsdk activate latest

:: Build WOS
@CALL emcc -std=c++17 -O3 -fno-exceptions -fno-rtti -fomit-frame-pointer ^
    -ffunction-sections -fno-trapping-math -fno-math-errno -fno-signed-zeros ^
    -W -Wall -pthread -lGL -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD ^
    -s MAX_WEBGL_VERSION=2 -o wos.js ^
    System/SysMessage.cpp ^
    System/SysCPU.cpp ^
    System/SysClock.cpp ^
    System/SysThread.cpp ^
    main.cpp

:: System pause
PAUSE
