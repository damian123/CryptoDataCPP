add_library(streaming STATIC 
  StreamingMarketData.cpp
  StreamingMarketData.h
)

target_include_directories(streaming PUBLIC ws $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(streaming ws OpenSSL::SSL OpenSSL::Crypto ws util ${Boost_LIBRARIES} Boost::random)
