include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(RestTest RestTest.cpp)
target_link_libraries(RestTest rest util ${CONAN_LIBS})
set_target_properties(RestTest PROPERTIES LINK_FLAGS "/ignore:4099")

add_executable(WSTest WSTest.cpp)
target_link_libraries(WSTest ws util ${CONAN_LIBS})
set_target_properties(WSTest PROPERTIES LINK_FLAGS "/ignore:4099")
