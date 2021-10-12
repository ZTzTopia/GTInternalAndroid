if(DEFINED _TOOLCHAIN_UTILITIES_COMMON)
  return()
else()
  set(_TOOLCHAIN_UTILITIES_COMMON 1)
endif()

if(NOT _TOOLCHAIN_NAME)
  message(FATAL_ERROR "_TOOLCHAIN_NAME is empty!")
endif()

if(NOT _TOOLCHAIN_TAG)
  message(FATAL_ERROR "_TOOLCHAIN_TAG is empty!")
endif()

message(STATUS "Toolchain: ${_TOOLCHAIN_NAME}")
message(STATUS "Toolchain tag: ${_TOOLCHAIN_TAG}")
