if(DEFINED _TOOLCHAIN_FLAGS_VS_CXX)
  return()
else()
  set(_TOOLCHAIN_FLAGS_VS_CXX 1)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../_utilities/_add_cache_flags.cmake")

_add_cache_flag(CMAKE_CXX_FLAGS_INIT "/std:c++${TOOLCHAIN_CXX_STANDARD}")

# Set CMAKE_CXX_STANDARD to cache to override project local value if present.
# FORCE added in case CMAKE_CXX_STANDARD already set in cache
# (e.g. set before 'project' by user).
set(CMAKE_CXX_STANDARD ${TOOLCHAIN_C_STANDARD} CACHE STRING "C++ Standard (toolchain)" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED YES CACHE BOOL "C++ Standard required" FORCE)
set(CMAKE_CXX_EXTENSIONS NO CACHE BOOL "C++ Standard extensions" FORCE)
