if(DEFINED _TOOLCHAIN_CLANG_EXAMPLE)
  return()
else()
  set(_TOOLCHAIN_CLANG_EXAMPLE 1)
endif()

set(_TOOLCHAIN_C_STANDARD 11)
set(_TOOLCHAIN_CXX_STANDARD 11)

include("${CMAKE_CURRENT_LIST_DIR}/../_utilities/_init.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/../_utilities/_generator.cmake")

_init("clang")

# TODO: need to check if generator visual studio,
# Visual Studio support clang toolset
_generator_not_permitted("Visual Studio")

include("${CMAKE_CURRENT_LIST_DIR}/../_utilities/_common.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/../_flags/_common.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/../_compiler/_clang.cmake")
