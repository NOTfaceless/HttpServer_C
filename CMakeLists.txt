cmake_minimum_required(VERSION 3.5)
project(HttpSever_C C)
add_executable(server.exe main.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}./)
target_include_directories(server.exe PRIVATE ${PROJECT_SOURCE_DIR}/include)

