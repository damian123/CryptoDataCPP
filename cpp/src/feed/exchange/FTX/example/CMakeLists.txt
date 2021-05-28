find_package(nlohmann_json CONFIG REQUIRED)

add_executable(RestTest RestTest.cpp)
target_link_libraries(RestTest rest OpenSSL::SSL OpenSSL::Crypto util ${Boost_LIBRARIES} nlohmann_json nlohmann_json::nlohmann_json)

add_executable(WSTest WSTest.cpp)
target_link_libraries(WSTest OpenSSL::SSL OpenSSL::Crypto ws util ${Boost_LIBRARIES} Boost::random fmt::fmt)
