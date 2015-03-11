include_directories (${CMAKE_SOURCE_DIR}/lib/)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable (segfit main.cpp vpabcFit.hpp vpabcFit.cpp segFit.cpp segFit.hpp option.hpp option.cpp)

target_link_libraries(segfit lbfgs)

