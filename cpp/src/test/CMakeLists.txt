enable_testing()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include(GoogleTest)

add_executable(cryptodata_test
    main.cpp
    staticdata/testsecurities.cpp
    marketdata/testStreamingMarketData.cpp
    marketdata/testHistoricalMarketData.cpp
)

target_link_libraries(cryptodata_test PRIVATE 
    securities
    streaming
    historical
    ws
    rest
    ${CONAN_LIBS}
)

set_target_properties(cryptodata_test 
                      PROPERTIES 
                      LINK_FLAGS "/ignore:4099"
)

gtest_discover_tests(cryptodata_test)
