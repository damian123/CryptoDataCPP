cmake_minimum_required(VERSION 3.20)
project(cryptodata CXX)

option(BUILD_TESTS_CRYPTODATA "Build the tests." OFF)
option(BUILD_EXAMPLES_CRYPTODATA "Build obsolete live-FTX examples." OFF)

set(CRYPTODATA_MAJOR_VERSION 1)
set(CRYPTODATA_MINOR_VERSION 0)
set(CRYPTODATA_PATCH_VERSION 0)
set(CRYPTODATA_VERSION
    ${CRYPTODATA_MAJOR_VERSION}.${CRYPTODATA_MINOR_VERSION}.${CRYPTODATA_PATCH_VERSION})

if (NOT EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  message(FATAL_ERROR
          "conanbuildinfo.cmake is missing. Run the pinned Conan 1 install from docs/Build.md.")
endif()
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS TARGETS)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR
    include
    CACHE PATH "Installation directory for header files")
set(DEF_INSTALL_CRYPTODATA_CMAKE_DIR lib/cmake/cryptodata)
set(INSTALL_CRYPTODATA_CMAKE_DIR
    ${DEF_INSTALL_CRYPTODATA_CMAKE_DIR}
    CACHE PATH "Installation directory for cmake files")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (WIN32)
  add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()
if (MSVC)
  add_compile_options(/EHsc)
endif()

include_directories(cpp/src/feed/exchange/FTX)
include_directories(SYSTEM cpp/src/external)
include_directories(SYSTEM cpp/src/external/websocketpp)

add_subdirectory(cpp/src/feed/exchange/FTX/util)
add_subdirectory(cpp/src/feed/exchange/FTX/rest)
add_subdirectory(cpp/src/feed/exchange/FTX/ws)

if (BUILD_EXAMPLES_CRYPTODATA)
  message(WARNING "Building obsolete examples that contact the defunct FTX service")
  add_subdirectory(cpp/src/feed/exchange/FTX/example)
endif()

add_subdirectory(cpp/src/staticdata/securities)
add_subdirectory(cpp/src/marketdata/streaming)
add_subdirectory(cpp/src/marketdata/historical)

if (BUILD_TESTS_CRYPTODATA)
  message(STATUS "Building tests")
  add_subdirectory(cpp/src/test ${CMAKE_BINARY_DIR}/bin)
endif()
