add_library(rest STATIC client.cpp client.h)
target_include_directories(rest PUBLIC)
target_link_libraries(rest)
