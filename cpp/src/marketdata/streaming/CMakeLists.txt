add_library(streaming STATIC 
  StreamingMarketData.cpp
  StreamingMarketData.h
)

target_include_directories(streaming PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(streaming PUBLIC ws CONAN_PKG::spdlog CONAN_PKG::fmt)
