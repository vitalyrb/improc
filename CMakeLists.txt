cmake_minimum_required(VERSION 3.5)

project(argus LANGUAGES CXX VERSION 1.0.0)

option(BUILD_UNIT_TESTING "Enable unit tests building" OFF)
option(RUN_UNIT_TESTS "Enable unit tests running after building." OFF)

set(CMAKE_CXX_FLAGS "-std=c++20 -g -Wall -Wextra")

include(cmake/SubDirectoryListFunction.cmake)

find_package(OpenCV REQUIRED)

#............... section: building libraries ................

include_directories("${PROJECT_SOURCE_DIR}/libraries")
include_directories("${PROJECT_SOURCE_DIR}/common")

SUBDIRLIST(LIBRARIES "${PROJECT_SOURCE_DIR}/libraries")

FOREACH(subdir ${LIBRARIES})
    message("Building ${subdir} library")
    file(GLOB_RECURSE ${subdir}_LIB_SOURCES "libraries/${subdir}/*.cpp") #collect library source file
    file(GLOB_RECURSE ${subdir}_TESTS "libraries/${subdir}/*.cxx") #collect unit tests source file
    list (APPEND TEST_SRC ${${subdir}_TESTS})
    add_library(${subdir} SHARED ${${subdir}_LIB_SOURCES})
    target_link_libraries(${subdir} ${OpenCV_LIBS})
    install (TARGETS ${subdir} DESTINATION lib)
ENDFOREACH()

#............... section: building main application  ................

add_subdirectory(sources)

#............... section: building unit tests  ................

if(NOT BUILD_UNIT_TESTING)
    message(STATUS "Unit-tests off.")
else()
  message(STATUS "Unit-tests on.")
  file(GLOB_RECURSE test_SOURCES "test/*.cpp")

  find_package(Catch2 REQUIRED)

  add_executable(run_unit_tests ${test_SOURCES})
  target_sources(run_unit_tests PRIVATE ${TEST_SRC})
  target_link_libraries(run_unit_tests Catch2::Catch2 ${OpenCV_LIBS})

  FOREACH(subdir ${LIBRARIES})
      target_link_libraries(run_unit_tests ${subdir})
  ENDFOREACH()

  include(CTest)
  include(Catch)
  catch_discover_tests(run_unit_tests)
  if(RUN_UNIT_TESTS)
    add_custom_target(check ALL COMMAND run_unit_tests)
  endif()
endif()

