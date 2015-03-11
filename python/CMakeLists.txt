FIND_PACKAGE(PythonLibs)
IF(PYTHONLIBS_FOUND)
  INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")
  MESSAGE(STATUS ${PYTHON_INCLUDE_DIRS})

  FIND_PACKAGE(Boost)
  IF(Boost_FOUND)
    INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}")
    SET(Boost_USE_STATIC_LIBS OFF)
    SET(Boost_USE_MULTITHREADED ON)
    SET(Boost_USE_STATIC_RUNTIME OFF)
    FIND_PACKAGE(Boost COMPONENTS python)

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
    include_directories (${CMAKE_SOURCE_DIR}/lib/)
    include_directories (${CMAKE_SOURCE_DIR}/src/)
    ADD_LIBRARY(
      pysegfit MODULE
      pysegfit.cpp
      ${CMAKE_SOURCE_DIR}/src/segFit.cpp
      ${CMAKE_SOURCE_DIR}/src/segFit.hpp
      ${CMAKE_SOURCE_DIR}/src/vpabcFit.cpp
      ${CMAKE_SOURCE_DIR}/src/vpabcFit.hpp
      ${CMAKE_SOURCE_DIR}/lib/solver.f
      ${CMAKE_SOURCE_DIR}/lib/program.cpp
      ${CMAKE_SOURCE_DIR}/lib/program.h
      )
    set_target_properties(pysegfit
      PROPERTIES
      SUFFIX ".so")
    TARGET_LINK_LIBRARIES(pysegfit ${Boost_LIBRARIES})
    
  ELSEIF(NOT Boost_FOUND)
    MESSAGE(WARNING "Unable to find Boost library. Will not build python wrap-up. (see README to install dependency)")
  ENDIF()
  
ELSE()
  MESSAGE(WARNING "Unable to find PythonLibs. Will not install python wrap-up. (see README to install dependency)")
ENDIF()


