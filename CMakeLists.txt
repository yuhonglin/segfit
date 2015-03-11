cmake_minimum_required (VERSION 2.6)

enable_language(Fortran)

project (SegFit)

set(CMAKE_BUILD_TYPE Release)

# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG")

add_subdirectory (lib)
add_subdirectory (src)
add_subdirectory (python)
# add_subdirectory (test)