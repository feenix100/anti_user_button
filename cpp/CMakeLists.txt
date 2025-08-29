cmake_minimum_required(VERSION 3.20)
project(antiuserbutton LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(antiuserbutton WIN32 main.cpp)

if (MINGW)
    target_link_libraries(antiuserbutton PRIVATE user32 gdi32)
endif()
