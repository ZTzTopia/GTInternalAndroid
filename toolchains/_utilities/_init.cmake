if(DEFINED _TOOLCHAIN_UTILITIES_INIT)
  return()
else()
  set(_TOOLCHAIN_UTILITIES_INIT 1)
endif()

macro(_init)
  set(_TOOLCHAIN_NAME "${ARGV}")
  set(_TOOLCHAIN_NAME_ALT "${ARGV}")

  if(_TOOLCHAIN_GNU_STANDARD AND _TOOLCHAIN_C_STANDARD)
    message(ERROR_FATAL "_TOOLCHAIN_GNU_STANDARD and _TOOLCHAIN_C_STANDARD defined! choose one of them.")
  endif()

  if(_TOOLCHAIN_GNUXX_STANDARD AND _TOOLCHAIN_CXX_STANDARD)
    message(ERROR_FATAL "_TOOLCHAIN_GNUXX_STANDARD and _TOOLCHAIN_CXX_STANDARD defined! choose one of them.")
  endif()

  if(NOT _TOOLCHAIN_GNU_STANDARD)
    if(_TOOLCHAIN_C_STANDARD)
      if(CMAKE_GENERATOR MATCHES "^Visual Studio*$")
        set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / vs${_TOOLCHAIN_C_STANDARD} support")
      else()
        set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / c${_TOOLCHAIN_C_STANDARD} support")
      endif()
    endif()
  else()
    set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / gnu${_TOOLCHAIN_GNU_STANDARD} support")
  endif()

  if(NOT _TOOLCHAIN_GNUXX_STANDARD)
    if(_TOOLCHAIN_CXX_STANDARD)
      if(CMAKE_GENERATOR MATCHES "^Visual Studio*$")
        set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / vs++${_TOOLCHAIN_CXX_STANDARD} support")
      else()
        set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / c++${_TOOLCHAIN_CXX_STANDARD} support")
      endif()
    endif()
  else()
    set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / gnu++${_TOOLCHAIN_GNUXX_STANDARD} support")
  endif()

  if(_TOOLCHAIN_NAME_ALT MATCHES "^NDK Android*$")
    set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / ${ANDROID_NDK_VERSION}")

    if(CMAKE_SYSTEM_VERSION)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / API ${CMAKE_SYSTEM_VERSION}")
    endif()

    if(CMAKE_ANDROID_ARCH_ABI)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / ${CMAKE_ANDROID_ARCH_ABI}")
    endif()

    if(CMAKE_ANDROID_ARM_NEON)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / Neon")
    endif()

    if(CMAKE_ANDROID_ARM_MODE)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / 32-bit ARM")
    endif()
  endif()

  if(CMAKE_GENERATOR MATCHES "^Visual Studio*$")

  else()
    if(_TOOLCHAIN_FPIC)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / Position-Independent Code")
    endif()

    if(_TOOLCHAIN_HIDDEN)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / hidden")
    endif()

    if(_TOOLCHAIN_FSECTIONS)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / function-sections")
    endif()

    if(_TOOLCHAIN_DSECTIONS)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / data-sections")
    endif()

    if(_TOOLCHAIN_STATIC)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / static")
    endif()

    if(_TOOLCHAIN_STATIC_STD)
      set(_TOOLCHAIN_NAME "${_TOOLCHAIN_NAME} / static (libgcc, libstdc++)")
    endif()
  endif()

  get_filename_component(_TOOLCHAIN_TAG "_init.cmake" NAME_WE)
endmacro()