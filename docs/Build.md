# prerequisites
Visual Studio 2019
Conan dependency and package manager `pip install conan`
(optional) Update submodules `git submodule update --init --recursive` or `git pull --recurse-submodules`


# Build
Open a x64 Native Tools Command Prompt for VS 2019
mkdir build
cd build
conan install .. -s compiler="Visual Studio" -s compiler.version=16 -sbuild_type=Debug
cmake .. -DBUILD_TESTS=on -DCMAKE_INSTALL_PREFIX=install -DCMAKE_BUILD_TYPE=Debug
cmake --build . --parallel --config Debug


TODO: Fix the issue that cryptodata_test can't find the dependent .dlls since it is in a seperate bin directory. 
A workraound is to add this folder to the path
set PATH=C:\dev\trading\CryptoData\build\bin;%PATH%

# Test
`cd build\bin`
`ctest`