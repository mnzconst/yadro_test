cmake_minimum_required(VERSION 3.28)
project(yadro_test)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra-semi -g -fsanitize=address -fno-omit-frame-pointer")
endif()

add_executable(yadro_test src/main.cpp)
