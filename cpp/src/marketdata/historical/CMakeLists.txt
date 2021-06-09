include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(historical STATIC
  HistMarketData.cpp
  HistMarketData.h
)

target_include_directories(historical PUBLIC rest $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(historical)
