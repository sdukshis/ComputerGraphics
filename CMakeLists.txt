cmake_minimum_required(VERSION 2.8)

project(render C)

include(CTest)

# Code Coverage Configuration
# add_library(coverage_config INTERFACE)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    add_definitions(-Wall)
    add_definitions(-Werror)
    # target_compile_options(coverage_config INTERFACE
    #     -O0        # no optimization
    #     -g         # generate debug info
    #     --coverage # sets all required flags
    # )
    # target_link_libraries(coverage_config INTERFACE --coverage)
else() #  MSVC
    add_definitions(/W4)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()


add_executable(render main.c model.c tga.c)
add_executable(julia julia.c model.c tga.c complex.c)
add_executable(mesh mesh.c model.c tga.c)

# target_compile_features(render PUBLIC c_std_11)
# target_compile_features(julia PUBLIC c_std_11)
# target_compile_features(mesh PUBLIC c_std_11)

if (UNIX)
    set(LIBS
        m
    )
endif()

target_link_libraries(render ${LIBS})
target_link_libraries(julia ${LIBS})
target_link_libraries(mesh ${LIBS})

if (BUILD_TESTING)
    list(APPEND test_srcs tests/read_obj.c)
    list(APPEND test_args "obj/cat.obj 3954 2248")

    list(APPEND test_srcs tests/read_tga.c)
    list(APPEND test_args "obj/cat_diff.tga 512 1024")

    list(APPEND test_srcs tests/write_tga.c)
    list(APPEND test_args "out.tga")

    list(LENGTH test_srcs count)
    math(EXPR count "${count}-1")
    foreach(i RANGE ${count})
      list(GET test_srcs ${i} src)
      list(GET test_args ${i} args)
      
      string(REPLACE " " ";" args_list ${args})

      get_filename_component(test_filename ${src} NAME_WE)
      set(test_name "test_${test_filename}")
      add_executable(${test_name} ${src} model.c tga.c)
      target_include_directories(${test_name} PUBLIC ${PROJECT_SOURCE_DIR})
    #   target_link_libraries(${test_name} PUBLIC coverage_config ${LIBS})
    #   target_compile_features(${test_name} PUBLIC c_std_11)
      add_test(
          NAME ${test_name}
          COMMAND $<TARGET_FILE:${test_name}> ${args_list}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  
      )
    endforeach()   
endif()
