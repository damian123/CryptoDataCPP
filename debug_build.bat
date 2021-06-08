@echo off
if not exist build mkdir build
cd build
conan install .. -s compiler="Visual Studio" -s compiler.version=16 -sbuild_type=Debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS_CRYPTODATA=on
cmake --build . --parallel --config Debug
