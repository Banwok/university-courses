cmake_minimum_required(VERSION 3.20)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)
add_executable(untitled main.cpp "Two kingdoms.cpp" triang.cpp fixed_set.h k_stat.cpp search_tree_count.cpp rectangles.cpp binary_tree.cpp ducha.cpp shifr.cpp magic.cpp directors.cpp intel_wall.cpp intel_uniq.cpp intel_tree.cpp intel_data.cpp max_substr.cpp period_max.cpp prefix_fun.cpp fuzzy_matches.cpp diff_substr.cpp nenoku.cpp burrows_wheeler.cpp parcm.cpp bisoprtazj.cpp reduction.cpp)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()