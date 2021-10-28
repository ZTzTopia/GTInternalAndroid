cmake_minimum_required(VERSION 3.0)

project(ModMenu VERSION 0.0.2)

ENABLE_LANGUAGE(C)
ENABLE_LANGUAGE(CXX)

FILE(GLOB FILE_INCLUDES
        src/*.h*
        src/game/*.h*
        src/game/*.h*
        src/gui/*.h*
        src/include/Substrate/*.h*
        src/include/And64InlineHook/*.h*
        src/include/KittyMemory/*.h*
        src/include/libmem/*.h*
        src/utilities/*.h*
        vendor/imgui/*.h*
        vendor/imgui/backends/*.h*
        vendor/enet/include/enet/*.h*)

FILE(GLOB FILE_SOURCES
        src/*.c*
        src/game/*.c*
        src/gui/*.c*
        src/include/Substrate/*.c*
        src/include/And64InlineHook/*.c*
        src/include/KittyMemory/*.c*
        src/include/libmem/*.c*
        src/utilities/*.c*
        vendor/imgui/*.c*
        vendor/imgui/backends/*.c*)

add_library(${PROJECT_NAME} SHARED
        ${FILE_INCLUDES}
        ${FILE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/enet/include)

target_link_libraries(${PROJECT_NAME} PRIVATE log android EGL GLESv2)

set(BUILD_TYPE_COMPILER_FLAGS
        -Wno-error=format-security
        -fpermissive
        -w
        -Werror
        -s
        -fno-rtti
        -fno-exceptions
        -fms-extensions
        -Wno-error=c++11-narrowing)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(BUILD_TYPE_COMPILER_FLAGS ${BUILD_TYPE_COMPILER_FLAGS})
  set(BUILD_TYPE_LINKER_FLAGS )
else()
  set(BUILD_TYPE_COMPILER_FLAGS
          ${BUILD_TYPE_COMPILER_FLAGS}
          -fvisibility=hidden
          -fvisibility-inlines-hidden
          -g0
          -O3
          -fomit-frame-pointer
          -ffunction-sections
          -fdata-sections)
  if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(BUILD_TYPE_LINKER_FLAGS
            -Wl
            -dead_strip)
  else()
    set(BUILD_TYPE_LINKER_FLAGS
            -Wl
            --strip-all)
  endif()
endif()

target_compile_options(${PROJECT_NAME} PUBLIC ${BUILD_TYPE_COMPILER_FLAGS})

if(CMAKE_VERSION VERSION_LESS 3.13)
  set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS ${BUILD_TYPE_LINKER_FLAGS})
else()
  target_link_options(${PROJECT_NAME} PUBLIC ${BUILD_TYPE_LINKER_FLAGS})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)