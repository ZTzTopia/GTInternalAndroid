if(DEFINED _TOOLCHAIN_FLAGS_FPIC)
  return()
else()
  set(_TOOLCHAIN_FLAGS_FPIC 1)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../_utilities/_add_cache_flags.cmake")

string(COMPARE EQUAL "${ANDROID_NDK_VERSION}" "" _not_android)
if(_not_android)
  _add_cache_flag(CMAKE_CXX_FLAGS "-fPIC")
  _add_cache_flag(CMAKE_C_FLAGS "-fPIC")
  _add_cache_flag(CMAKE_Fortran_FLAGS "-fPIC")
else()
  _add_cache_flag(CMAKE_CXX_FLAGS_INIT "-fPIC")
  _add_cache_flag(CMAKE_C_FLAGS_INIT "-fPIC")
  _add_cache_flag(CMAKE_Fortran_FLAGS_INIT "-fPIC")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE CACHE BOOL "Position independent code" FORCE)

# Linux, GCC 7.3.0 same results with and without '-fPIC' flag for code:
#
#  #include <iostream>
#  int main() {
#  #if defined(__PIC__)
#    std::cout << "PIC: " << __PIC__ << std::endl;
#  #else
#    std::cout << "PIC not defined" << std::endl;
#  #endif
#  }
