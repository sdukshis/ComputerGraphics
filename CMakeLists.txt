cmake_minimum_required(VERSION 2.8)

project(render C)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_definitions(-Wall)
    add_definitions(-Werror)
else() #  MSVC
    add_definitions(/W4)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(/Ze) # C4204
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