add_library(historical STATIC
  HistMarketData.cpp
  HistMarketData.h
)

target_include_directories(historical PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(historical PUBLIC rest CONAN_PKG::fmt)
