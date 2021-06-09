cmake_minimum_required(VERSION 3.10)
project(cryptodata CXX)

option(BUILD_TESTS_CRYPTODATA "Build the tests." OFF)
option(BUILD_DOCS "Build the documentation" OFF)
#option(CRYPTODATA_PEDANTIC "Enable extra warnings and expensive tests." OFF)
#option(CRYPTODATA_WERROR   "Halt the compilation with an error on compiler warnings." OFF)

set(CRYPTODATA_MAJOR_VERSION 1)
set(CRYPTODATA_MINOR_VERSION 0)
set(CRYPTODATA_PATCH_VERSION 0)
set(CRYPTODATA_VERSION
    ${CRYPTODATA_MAJOR_VERSION}.${CRYPTODATA_MINOR_VERSION}.${CRYPTODATA_PATCH_VERSION})

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

set(CMAKE_CXX_STANDARD 20)
if (WIN32)
  set(CMAKE_CXX_FLAGS "-D_WIN32_WINNT=0x0601 /EHsc")
endif()

include_directories(cpp/src/feed/exchange/FTX)
include_directories(SYSTEM cpp/src/external)
include_directories(SYSTEM cpp/src/external/websocketpp)

add_subdirectory(cpp/src/feed/exchange/FTX/rest)
add_subdirectory(cpp/src/feed/exchange/FTX/ws)
add_subdirectory(cpp/src/feed/exchange/FTX/util)
add_subdirectory(cpp/src/feed/exchange/FTX/example)

add_subdirectory(cpp/src/staticdata/securities)
add_subdirectory(cpp/src/marketdata/streaming)
add_subdirectory(cpp/src/marketdata/historical)

if (BUILD_TESTS_CRYPTODATA)
  message(STATUS "Building tests")
  add_subdirectory(cpp/src/test ${CMAKE_BINARY_DIR}/bin)
endif()

if (BUILD_DOCS)
  message(STATUS "Building documentation")
  add_subdirectory(cpp/docs)
endif()
