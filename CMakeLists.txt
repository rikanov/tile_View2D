cmake_minimum_required(VERSION 2.6)
project(view2d)

add_executable(view2d main.cpp view2d.cpp players.cpp cursor.cpp)

install(TARGETS view2d RUNTIME DESTINATION bin)
