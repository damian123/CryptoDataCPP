include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(streaming STATIC 
  StreamingMarketData.cpp
  StreamingMarketData.h
)

target_include_directories(streaming PUBLIC ws $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(streaming)

