git submodule add https://github.com/Microsoft/vcpkg cpp/src/external/vcpkg
git submodule add https://github.com/MuwazanaSA/websocketpp.git cpp/src/external/websocketpp

Open a x64 Native Tools Command Prompt for VS 2019
cd cpp
mkdir build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\dev\trading\CryptoData\cpp\src\external\vcpkg\scripts\buildsystems\vcpkg.cmake