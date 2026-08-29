add_library(rest STATIC client.cpp client.h)
target_include_directories(rest PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(rest PUBLIC util)
