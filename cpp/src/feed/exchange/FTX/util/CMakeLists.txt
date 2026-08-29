add_library(util STATIC 
  HTTP.cpp
  HTTP.h  
  WS.cpp
  WS.h
  TimeUtil.h
  Encoding.cpp
  Encoding.h
  Secret.cpp
  Secret.h)

target_include_directories(util PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(util PUBLIC CONAN_PKG::boost CONAN_PKG::openssl CONAN_PKG::fmt)
