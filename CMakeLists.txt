# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

cmake_minimum_required(VERSION 3.1...3.15)

project( evaluation.out )
project( gradient.out )

# CGAL and its components
find_package( CGAL REQUIRED OPTIONAL_COMPONENTS Qt5 )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

# include for local directory

# include for local package


# Creating entries for target: evaluation.out
# ############################

add_executable( evaluation.out  evaluation.cpp )
add_executable( gradient.out gradient.cpp )

add_to_cached_list( CGAL_EXECUTABLE_TARGETS evaluation.out )
add_to_cached_list( CGAL_EXECUTABLE_TARGETS gradient.out )
# Link the executable to CGAL and third-party libraries
target_link_libraries(evaluation.out PRIVATE CGAL::CGAL )
target_link_libraries(evaluation.out PUBLIC CGAL::CGAL_Basic_viewer)
target_link_libraries(gradient.out PRIVATE CGAL::CGAL )
target_link_libraries(gradient.out PUBLIC CGAL::CGAL_Basic_viewer)