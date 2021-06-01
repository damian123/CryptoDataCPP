include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(rest STATIC client.cpp client.h)
target_include_directories(rest PUBLIC)
target_link_libraries(rest)
