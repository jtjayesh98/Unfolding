cmake_minimum_required(VERSION 3.1...3.22)
project(Algebraic_kernel_d_Examples)

find_package(CGAL REQUIRED COMPONENTS Core)

find_package(MPFI QUIET)

if(MPFI_FOUND AND NOT CGAL_DISABLE_GMP)
  include(${MPFI_USE_FILE})
  include(CGAL_VersionUtils)
  create_single_source_cgal_program("Compare_1.cpp")
  create_single_source_cgal_program("Construct_algebraic_real_1.cpp")
  create_single_source_cgal_program("Isolate_1.cpp")
  create_single_source_cgal_program("Sign_at_1.cpp")
  create_single_source_cgal_program("Solve_1.cpp")
else()
    message(STATUS "This program requires the CGAL, CGAL_Core and MPFI libraries, and will not be compiled.")
endif()
