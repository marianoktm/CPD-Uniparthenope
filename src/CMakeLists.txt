cmake_minimum_required(VERSION 3.17)
project(ProgettoCalcolo C)

set(CMAKE_C_STANDARD 99)

include_directories(/usr/local/include)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xclang -fopenmp")
find_library(OPENMP_LIB libomp.dylib /usr/local/lib)


add_executable(ProgettoCalcolo main.c p21fun.c p21fun.h)
add_executable(mtxtxt_create p21fun.c p21fun.h mtxtxt.c)


target_link_libraries(ProgettoCalcolo LINK_PUBLIC ${OPENMP_LIB})
