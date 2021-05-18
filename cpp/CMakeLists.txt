cmake_minimum_required(VERSION 3.10)
project(cryptodata)

set(CMAKE_CXX_STANDARD 17)
if (UNIX)
	set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors -Wpedantic -Wextra -Wmissing-include-dirs -Wconversion -Wsign-conversion -pthread")
	add_compile_options(-Wall -Werror -Wfatal-errors -Wpedantic -Wextra -Wmissing-include-dirs -Wconversion -Wsign-conversion -pthread)

	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -march=native -g")
elseif (WIN32)
	set(CMAKE_CXX_FLAGS "-D_WIN32_WINNT=0x0601 /EHsc")
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost 1.71 REQUIRED COMPONENTS random)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

find_package(OpenSSL 1.1 REQUIRED)
include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})

include_directories(src/feed/exchange/FTX)
include_directories(SYSTEM src/external)

add_subdirectory(src/feed/exchange/FTX/rest)
add_subdirectory(src/feed/exchange/FTX/ws)
add_subdirectory(src/feed/exchange/FTX/util)
add_subdirectory(src/feed/exchange/FTX/example)
