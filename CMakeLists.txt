cmake_minimum_required(VERSION 3.22)
project(LUT_SRAM_synthesis)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
include_directories("header files")
include_directories("source files")

add_executable(LUT_SRAM_synthesis
        "header files/divide_Terms_of_Function.h"
        "header files/get_Terms_of_Function.h"
        "header files/Global.h"
        "header files/logical_Combination.h"
        "header files/logical_iteration.h"
        "header files/optimize_Methods.h"
        "header files/spatial_Combination.h"
        "source files/blossom_method.cpp"
        "source files/divide_Terms_of_Function.c"
        "source files/get_Terms_of_Function.c"
        "source files/logical_Combination.c"
        "source files/logical_iteration.c"
        "source files/main.c"
        "source files/optimize_Methods.c"
        "source files/spatial_Combination.c")
