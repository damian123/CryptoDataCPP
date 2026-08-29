@echo off
set CONAN_REVISIONS_ENABLED=1
if not exist build mkdir build
cd build
conan lock create ..\conanfile.txt --lockfile ..\conan.lock --lockfile-out conan-debug.lock --build=missing -s compiler="Visual Studio" -s compiler.version=17 -s build_type=Debug
conan install ..\conanfile.txt --lockfile conan-debug.lock --build=missing
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS_CRYPTODATA=on
cmake --build . --parallel --config Debug
