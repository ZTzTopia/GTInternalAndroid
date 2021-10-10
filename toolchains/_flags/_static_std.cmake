if(DEFINED _TOOLCHAIN_FLAGS_STATIC_STD)
  return()
else()
  set(_TOOLCHAIN_FLAGS_STATIC_STD 1)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../_utilities/_add_cache_flags.cmake")

_add_cache_flags(CMAKE_C_FLAGS "-static-libgcc")
_add_cache_flags(CMAKE_CXX_FLAGS "-static-libgcc")
_add_cache_flags(CMAKE_CXX_FLAGS "-static-libstdc++")
