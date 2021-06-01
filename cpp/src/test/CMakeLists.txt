include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Add test cpp file
add_executable(cryptodata_test
    main.cpp
    staticdata/testsecurities.cpp
    marketdata/testStreamingMarketData.cpp
)

target_include_directories(cryptodata_test PRIVATE securities streaming)

target_link_libraries(cryptodata_test PRIVATE 
  securities
  streaming
  ws
  rest
  ${CONAN_LIBS})

set_target_properties(cryptodata_test PROPERTIES LINK_FLAGS "/ignore:4099")

#add_test(
#    NAME cryptodata_test
#    COMMAND cryptodata_test
#)

