include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(securities STATIC 
  securities.cpp
  securities.h
  )

target_include_directories(securities PUBLIC rest $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(securities)
