add_executable(RestTest RestTest.cpp)
target_link_libraries(RestTest PRIVATE rest util)

add_executable(WSTest WSTest.cpp)
target_link_libraries(WSTest PRIVATE ws util)

if (MSVC)
  set_target_properties(RestTest WSTest PROPERTIES LINK_FLAGS "/ignore:4099")
endif()
