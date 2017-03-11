cmake_minimum_required(VERSION 2.8)

project(render C)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall)
    add_definitions(-Werror)
elseif(MSVC)
    add_definitions(/W4)
else()
    message("Unknown compiler.")
endif()

add_executable(render main.c model.c tga.c)
add_executable(julia julia.c model.c tga.c)

