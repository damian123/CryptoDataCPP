find_package(nlohmann_json CONFIG REQUIRED)

add_executable(rest_test rest_test.cc)
target_link_libraries(rest_test rest OpenSSL::SSL OpenSSL::Crypto util ${Boost_LIBRARIES} nlohmann_json nlohmann_json::nlohmann_json)

add_executable(ws_test ws_test.cc)
target_link_libraries(ws_test OpenSSL::SSL OpenSSL::Crypto ws util ${Boost_LIBRARIES} Boost::random)
