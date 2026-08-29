@echo off
set CONAN_REVISIONS_ENABLED=1
conan remote add conancenter-v2 https://center2.conan.io --force
if not exist build mkdir build
cd build
conan lock create ..\conanfile.txt --lockfile ..\conan.lock --lockfile-out conan-debug.lock --build=missing --remote conancenter-v2 -s compiler="Visual Studio" -s compiler.version=17 -s build_type=Debug
conan install ..\conanfile.txt --lockfile conan-debug.lock --build=missing --remote conancenter-v2
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS_CRYPTODATA=on
cmake --build . --parallel --config Debug
