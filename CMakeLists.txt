cmake_minimum_required(VERSION 3.0)

project(ModMenu VERSION 0.0.1 LANGUAGES CXX)

FILE(GLOB FILE_INCLUDES
        src/*.h* src/game/*.h*
        src/game/*.h
        src/gui/*.h*

        src/include/Substrate/hde64.c
        src/include/Substrate/SubstrateDebug.cpp
        src/include/Substrate/SubstrateHook.cpp
        src/include/Substrate/SubstratePosixMemory.cpp
        src/include/Substrate/SymbolFinder.cpp
        src/include/KittyMemory/*.h*

        src/utilities/*.h*

        vendor/imgui/*.h*)

FILE(GLOB FILE_SOURCES
        src/*.c*
        src/game/*.c*
        src/gui/*.c*

        src/include/Substrate/hde64.h
        src/include/Substrate/SubstrateDebug.hpp
        src/include/Substrate/SubstrateHook.h
        src/include/Substrate/CydiaSubstrate.h
        src/include/Substrate/SymbolFinder.h
        src/include/KittyMemory/*.c*

        src/utilities/*.c*

        vendor/imgui/*.c*)

add_library(${PROJECT_NAME}
        ${FILE_INCLUDES}
        ${FILE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE vendor/imgui)

target_link_libraries(${PROJECT_NAME} PRIVATE log android EGL GLESv1_CM GLESv2)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(BUILD_TYPE_COMPILER_FLAGS
          -g3)
  set(BUILD_TYPE_LINKER_FLAGS )
else()
  set(BUILD_TYPE_COMPILER_FLAGS
          -Wno-error=format-security
          -fvisibility=hidden
          -fvisibility-inlines-hidden
          -fomit-frame-pointer
          -ffunction-sections
          -fdata-sections
          -g0
          -Ofast
          -DNDEBUG)
  set(BUILD_TYPE_LINKER_FLAGS
          -Wl
          --gc-sections
          --strip-all)
endif()

target_compile_options(${PROJECT_NAME} PUBLIC
        -Wno-error=format-security
        -fpermissive
        -fno-rtti
        -fno-exceptions
        ${BUILD_TYPE_COMPILER_FLAGS})

if(CMAKE_VERSION VERSION_LESS 3.13)
  set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS ${BUILD_TYPE_LINKER_FLAGS})
else()
  target_link_options(${PROJECT_NAME} PUBLIC
          ${BUILD_TYPE_LINKER_FLAGS})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)