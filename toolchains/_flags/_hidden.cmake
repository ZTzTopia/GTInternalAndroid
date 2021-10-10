if(DEFINED _TOOLCHAIN_FLAGS_HIDDEN)
  return()
else()
  set(_TOOLCHAIN_FLAGS_HIDDEN 1)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../_utilities/_add_cache_flags.cmake")

string(COMPARE EQUAL "${ANDROID_NDK_VERSION}" "" _not_android)
if(_not_android)
  _add_cache_flags(CMAKE_CXX_FLAGS "-fvisibility=hidden")
  _add_cache_flags(CMAKE_CXX_FLAGS "-fvisibility-inlines-hidden") # only C++
  _add_cache_flags(CMAKE_C_FLAGS "-fvisibility=hidden")
else()
  _add_cache_flags(CMAKE_CXX_FLAGS_INIT "-fvisibility=hidden")
  _add_cache_flags(CMAKE_CXX_FLAGS_INIT "-fvisibility-inlines-hidden") # only C++ 
  _add_cache_flags(CMAKE_C_FLAGS_INIT "-fvisibility=hidden")
endif()
