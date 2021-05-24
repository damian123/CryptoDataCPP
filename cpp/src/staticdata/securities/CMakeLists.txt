find_package(nlohmann_json CONFIG REQUIRED)

add_library(securities STATIC 
  securities.cpp
  securities.h
  )

target_include_directories(securities PUBLIC rest $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(securities rest OpenSSL::SSL OpenSSL::Crypto util ${Boost_LIBRARIES} nlohmann_json nlohmann_json::nlohmann_json)
