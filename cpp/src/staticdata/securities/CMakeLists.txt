add_library(securities STATIC 
  securities.cpp
  securities.h
  )

target_include_directories(securities PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(securities PUBLIC rest CONAN_PKG::fmt)
