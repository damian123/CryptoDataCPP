# Historical Windows build

This procedure preserves the repository's legacy build path. It does not verify the defunct FTX service or make the project production-ready.

## Prerequisites

- Windows with Visual Studio 2022 and the Desktop development with C++ workload
- CMake 3.20 or newer
- Python 3.11
- Conan 1.66.0 (`python -m pip install "conan==1.66.0"`)

Conan 2 is intentionally unsupported: `conanfile.txt` uses the Conan 1 CMake generator and `conanbuildinfo.cmake` integration.

## Configure and build

Open an x64 Native Tools Command Prompt for Visual Studio 2022 and run:

```bat
git submodule update --init --recursive
python -m pip install "conan==1.66.0"
set CONAN_REVISIONS_ENABLED=1
conan remote add conancenter-v2 https://center2.conan.io --force

cmake -E make_directory build
cd build
conan lock create ..\conanfile.txt --lockfile ..\conan.lock --lockfile-out conan-debug.lock --build=missing --remote conancenter-v2 -s compiler="Visual Studio" -s compiler.version=17 -s build_type=Debug
conan install ..\conanfile.txt --lockfile conan-debug.lock --build=missing --remote conancenter-v2
cmake .. -DBUILD_TESTS_CRYPTODATA=on -DCMAKE_BUILD_TYPE=Debug
cmake --build . --parallel --config Debug
```

## Test

Conan imports runtime DLLs under the build output. If Windows cannot locate them, prepend that directory for the current shell before running CTest:

```bat
set PATH=%CD%\bin\bin;%PATH%
ctest --test-dir bin -C Debug --output-on-failure --timeout 15
```

Expected discovery: thirteen deterministic GoogleTest cases. The default build and test path never contacts FTX. Obsolete network examples require an explicit `-DBUILD_EXAMPLES_CRYPTODATA=on` and must not be run with credentials.

## Non-Windows hosts

The source now targets standard C++17 and avoids MSVC-only flags outside guarded blocks, but this archival repository still has no supported Linux or macOS distribution contract. The CI contract is the Windows configuration above.

`conan.lock` is a Windows cross-configuration base lock: it fixes direct, transitive, and source-build tool recipe revisions while allowing MSVC Debug and Release package IDs to differ. The lock-create command must retain `--build=missing` so its derived full lock includes the pinned B2, NASM, and Strawberry Perl build tools. Revisions must remain enabled whenever the lock is consumed. The explicit `conancenter-v2` remote is required because the legacy Conan Center endpoint does not contain the pinned Boost recipe revision with the working official archive URL. The lock improves repeatability but cannot guarantee that legacy recipes, sources, or hosted runners remain available indefinitely.
