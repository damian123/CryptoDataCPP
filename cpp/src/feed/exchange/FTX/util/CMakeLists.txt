add_library(util STATIC 
  HTTP.cpp
  HTTP.h  
  WS.cpp
  WS.h
  Time.h
  Encoding.h)

target_include_directories(util PUBLIC)
target_link_libraries(util OpenSSL::SSL OpenSSL::Crypto)
