cmake_minimum_required(VERSION 3.10)
project(cryptodata CXX)

# Determine if cryptodata is built as a subproject (using add_subdirectory)
# or if it is the master project.
if (NOT DEFINED CRYPTODATA_MASTER_PROJECT)
  set(CRYPTODATA_MASTER_PROJECT OFF)
  if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CRYPTODATA_MASTER_PROJECT ON)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
  endif ()
endif ()

option(CRYPTODATA_PEDANTIC "Enable extra warnings and expensive tests." OFF)
option(CRYPTODATA_WERROR   "Halt the compilation with an error on compiler warnings." OFF)

# Options that control generation of various targets.
option(CRYPTODATA_TEST "Generate the test target." ON) #${CRYPTODATA_MASTER_PROJECT})
option(CRYPTODATA_DOC "Generate the doc target" OFF) #${CRYPTODATA_MASTER_PROJECT})

# Joins arguments and places the results in ${result_var}.
function(join result_var)
  set(result )
  foreach (arg ${ARGN})
    set(result "${result}${arg}")
  endforeach ()
  set(${result_var} "${result}" PARENT_SCOPE)
endfunction()

# Get version from core.h
file(READ cpp/include/cryptodata/core.h core_h)
if (NOT core_h MATCHES "CRYPTODATA_VERSION ([0-9]+)([0-9][0-9])([0-9][0-9])")
  message(FATAL_ERROR "Cannot get CRYPTODATA_VERSION from core.h.")
endif ()
# Use math to skip leading zeros if any.
math(EXPR CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
math(EXPR CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
math(EXPR CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})
join(CRYPTODATA_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
message(STATUS "Crypto Data Version: ${CRYPTODATA_VERSION}")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 20)
if (WIN32)
  set(CMAKE_CXX_FLAGS "-D_WIN32_WINNT=0x0601 /EHsc")
elseif (UNIX)
	set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors -Wpedantic -Wextra -Wmissing-include-dirs -Wconversion -Wsign-conversion -pthread")
	add_compile_options(-Wall -Werror -Wfatal-errors -Wpedantic -Wextra -Wmissing-include-dirs -Wconversion -Wsign-conversion -pthread)

	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -march=native -g")
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost 1.71 REQUIRED COMPONENTS random)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

find_package(OpenSSL 1.1 REQUIRED)
include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})

find_package(TBB CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

include_directories(cpp/src/feed/exchange/FTX)
include_directories(SYSTEM cpp/src/external)
include_directories(SYSTEM cpp/src/external/websocketpp)

add_subdirectory(cpp/src/feed/exchange/FTX/rest)
add_subdirectory(cpp/src/feed/exchange/FTX/ws)
add_subdirectory(cpp/src/feed/exchange/FTX/util)
add_subdirectory(cpp/src/feed/exchange/FTX/example)

add_subdirectory(cpp/src/staticdata/securities)
add_subdirectory(cpp/src/marketdata/streaming)

if (CRYPTODATA_TEST)
  message(STATUS "Building tests")
  add_subdirectory(cpp/src/test)
endif()
if (CRYPTODATA_DOC)
  message(STATUS "Building documentaion")
  add_subdirectory(cpp/docs)
endif()
