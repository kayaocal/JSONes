cmake_minimum_required(VERSION 3.10)

# set the project name
project(Jsones DESCRIPTION "Json parsing")


add_executable(Jsones ${CMAKE_SOURCE_DIR}/test.cpp ${CMAKE_SOURCE_DIR}/Jsones.cpp ${CMAKE_SOURCE_DIR}/Jsones.h)



target_compile_features(Jsones PRIVATE cxx_std_11)