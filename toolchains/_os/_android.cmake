if(DEFINED _TOOLCHAIN_OS_ANDROID)
  return()
else()
  set(_TOOLCHAIN_OS_ANDROID 1)
endif()

if(NOT CMAKE_IS_ANDROID)
  message(FATAL_ERROR "Is not android build toolchain!")
endif()

if (ANDROID_NDK_ROOT)
  set(CMAKE_ANDROID_NDK "${ANDROID_NDK_ROOT}")
elseif (ANDROID_NDK)
  set(CMAKE_ANDROID_NDK "${ANDROID_NDK}")
endif()

string(COMPARE EQUAL "${CMAKE_SYSTEM_VERSION}" "" _is_empty)
if(_is_empty)
  message(FATAL_ERROR "CMAKE_SYSTEM_VERSION is not defined!")
endif()

set(CMAKE_SYSTEM_NAME Android)

if(CMAKE_VERSION VERSION_LESS 3.7.1)
  message(FATAL_ERROR "Minimum CMake version for Android is 3.7.1!")
endif()

# ANDROID macro is not defined by CMake 3.7+, however it is used by
# some packages like OpenCV
# (https://gitlab.kitware.com/cmake/cmake/merge_requests/62)
add_definitions("-DANDROID")

include(${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake)
