cmake_minimum_required(VERSION 2.8)

project(render C)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_definitions(-Wall)
    add_definitions(-Werror)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    add_definitions(/W4)
else()
    message("Unknown compiler.")
endif()


add_executable(render main.c model.c tga.c)
add_executable(julia julia.c model.c tga.c complex.c)

if (UNIX)
    set(LIBS
        m
    )
endif()

target_link_libraries(render ${LIBS})
target_link_libraries(julia ${LIBS})