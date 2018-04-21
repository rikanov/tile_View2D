cmake_minimum_required(VERSION 2.6)
project(view2d)

add_library(view2d STATIC view2d.cpp players.cpp cursor.cpp)
target_link_libraries(view2d /home/rikanov/projects/BoardView/build/libboardview.a)
install(TARGETS view2d RUNTIME DESTINATION bin ARCHIVE DESTINATION lib)
