cmake_minimum_required(VERSION 2.6)
project(view2d)

add_library(view2d STATIC view2d.cpp select.cpp players.cpp cursor.cpp)

install(TARGETS view2d RUNTIME DESTINATION bin ARCHIVE DESTINATION lib)
