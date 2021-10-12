if(DEFINED _TOOLCHAIN_COMPILER_CLANG)
  return()
else()
  set(_TOOLCHAIN_COMPILER_CLANG 1)
endif()

# find compiler
find_program(CMAKE_C_COMPILER clang)
find_program(CMAKE_CXX_COMPILER clang++)

if(NOT CMAKE_C_COMPILER)
  message(FATAL_ERROR "clang not found!")
endif()

if(NOT CMAKE_CXX_COMPILER)
  message(FATAL_ERROR "clang++ not found!")
endif()

set(CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" CACHE STRING "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}" CACHE STRING "C++ compiler" FORCE)
