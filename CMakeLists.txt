cmake_minimum_required(VERSION 3.10)
project(cryptodata CXX)

option(BUILD_TESTS_CRYPTODATA "Build the tests." ON)
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

if (BUILD_TESTS_CRYPTODATA)
  message(STATUS "Building tests")
  add_subdirectory(cpp/src/test ${CMAKE_BINARY_DIR}/bin)
endif()

if (BUILD_DOCS)
  message(STATUS "Building documentation")
  add_subdirectory(cpp/docs)
endif()

# Install
#install(TARGETS cryptodata
#                # IMPORTANT: Add the cryptodata library to the "export-set"
#        EXPORT CryptodataTargets
#        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
#        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
#        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/exports/cryptodata/exports.h
#        DESTINATION ${INSTALL_INCLUDE_DIR}/cryptodata)
#install(FILES ${public_utils_headers}
#        DESTINATION ${INSTALL_INCLUDE_DIR}/cryptodata/utils)

#include(CMakePackageConfigHelpers)
# ... for the install tree
#configure_package_config_file(
#  cmake/templates/CryptoDataConfig.cmake.in
#  ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CryptoDataConfig.cmake
#  INSTALL_DESTINATION
#  ${INSTALL_CRYPTODATA_CMAKE_DIR})

# version
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/CryptoDataConfigVersion.cmake
#  VERSION ${CRYPTODATA_VERSION}
#  COMPATIBILITY SameMajorVersion)

# Install the CryptoDataConfig.cmake and CryptoDataConfigVersion.cmake
#install(
#  FILES
#    "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CryptoDataConfig.cmake"
#    "${CMAKE_CURRENT_BINARY_DIR}/CryptoDataConfigVersion.cmake"
#    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/Findc-ares.cmake"
#  DESTINATION "${INSTALL_CRYPTODATA_CMAKE_DIR}"
#  COMPONENT dev)

# Install the export set for use with the install-tree
#install(EXPORT CryptoDataTargets
#        DESTINATION "${INSTALL_CRYPTODATA_CMAKE_DIR}"
#        NAMESPACE CryptoData::
#        COMPONENT dev)
