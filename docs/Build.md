`git submodule update --init --recursive`

Open a x64 Native Tools Command Prompt for VS 2019
cd cpp
mkdir build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\dev\trading\CryptoData\cpp\src\external\vcpkg\scripts\buildsystems\vcpkg.cmake