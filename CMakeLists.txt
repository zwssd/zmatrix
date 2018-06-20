cmake_minimum_required(VERSION 2.6)
project(zmatrix)

add_executable(zmatrix main.cpp)

install(TARGETS zmatrix RUNTIME DESTINATION bin)
