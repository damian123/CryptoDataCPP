enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

# Add test cpp file
add_executable(cryptodata_test
    main.cpp
    staticdata/testsecurities.cpp
)

target_include_directories(cryptodata_test PRIVATE securities)
target_link_libraries(cryptodata_test PRIVATE securities ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} TBB::tbb fmt::fmt)

add_test(
    NAME cryptodata_test
    COMMAND cryptodata_test
)
