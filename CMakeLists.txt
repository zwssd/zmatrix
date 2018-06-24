
cmake_minimum_required(VERSION 2.6)

project(zmatrix)

add_executable(zmatrix main.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ncurses)

install(TARGETS zmatrix RUNTIME DESTINATION bin)
