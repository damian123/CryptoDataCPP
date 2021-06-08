include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB SOURCES "*cpp" "*h")
add_library(ws STATIC ${SOURCES})
target_include_directories(ws PUBLIC)
target_link_libraries(ws util)
