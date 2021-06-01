add_executable(RestTest RestTest.cpp)
target_link_libraries(RestTest rest util)
set_target_properties(RestTest PROPERTIES LINK_FLAGS "/ignore:4099")

add_executable(WSTest WSTest.cpp)
target_link_libraries(WSTest ws util)
set_target_properties(WSTest PROPERTIES LINK_FLAGS "/ignore:4099")
