if(DEFINED _TOOLCHAIN_FLAGS_CXX)
  return()
else()
  set(_TOOLCHAIN_FLAGS_CXX 1)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../_utilities/_add_cache_flags.cmake")

string(COMPARE EQUAL "${ANDROID_NDK_VERSION}" "" _not_android)

if(CMAKE_GENERATOR MATCHES "^Visual Studio*$")
	_add_cache_flag("Use _flags/_vs-cxx.cmake instead")
elseif(_not_android)
	_add_cache_flag(CMAKE_CXX_FLAGS "-std=c++${TOOLCHAIN_CXX_STANDARD}")
else()
	_add_cache_flag(CMAKE_CXX_FLAGS_INIT "-std=c++${TOOLCHAIN_CXX_STANDARD}")
endif()

# Set CMAKE_CXX_STANDARD to cache to override project local value if present.
# FORCE added in case CMAKE_CXX_STANDARD already set in cache
# (e.g. set before 'project' by user).
set(CMAKE_CXX_STANDARD ${TOOLCHAIN_CXX_STANDARD} CACHE STRING "C++ Standard (toolchain)" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED YES CACHE BOOL "C++ Standard required" FORCE)
set(CMAKE_CXX_EXTENSIONS NO CACHE BOOL "C++ Standard extensions" FORCE)
