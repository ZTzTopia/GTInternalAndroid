if(DEFINED _TOOLCHAIN_OS_ANDROID)
  return()
else()
  set(_TOOLCHAIN_OS_ANDROID 1)
endif()

string(COMPARE EQUAL "${ANDROID_NDK_VERSION}" "" _is_empty)
if(_is_empty)
  message(FATAL_ERROR "ANDROID_NDK_VERSION is not defined!")
endif()

set(_env_ndk "$ENV{ANDROID_NDK_${ANDROID_NDK_VERSION}}")
string(COMPARE EQUAL "${_env_ndk}" "" _is_empty)
if(_is_empty)
  message(WARNING "Environment variable 'ANDROID_NDK_${ANDROID_NDK_VERSION}' not set!")

  set(_env_ndk "${ANDROID_NDK_${ANDROID_NDK_VERSION}}")
  string(COMPARE EQUAL "${_env_ndk}" "" _is_empty)
  if(_is_empty)
    message(FATAL_ERROR "ANDROID_NDK_${ANDROID_NDK_VERSION} is not defined!")
  endif()
endif()

set(ANDROID_NDK "${_env_ndk}")

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

# from ndk folder
include(${_env_ndk}/build/cmake/android.toolchain.cmake)
