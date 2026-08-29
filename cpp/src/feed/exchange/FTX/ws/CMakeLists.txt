add_library(ws STATIC client.cpp client.h)
target_include_directories(ws PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(ws PUBLIC util)
