enable_testing()
include(GoogleTest)

add_executable(cryptodata_test
  main.cpp
  staticdata/testsecurities.cpp
  marketdata/testStreamingMarketData.cpp
  marketdata/testHistoricalMarketData.cpp)

target_link_libraries(cryptodata_test PRIVATE
  securities
  streaming
  historical
  ws
  rest
  CONAN_PKG::gtest)

if (MSVC)
  set_target_properties(cryptodata_test PROPERTIES LINK_FLAGS "/ignore:4099")
endif()

gtest_discover_tests(
  cryptodata_test
  DISCOVERY_MODE PRE_TEST
  PROPERTIES TIMEOUT 10)
