cmake_minimum_required(VERSION 3.22)
project(GettingStarted)
find_package(glfw3 REQUIRED)
add_executable(out "HelloWindow.cpp" "../glad.c")  # create target by compiling and statically linking the given files
target_link_libraries(out glfw)  # dynamically link glfw library to target