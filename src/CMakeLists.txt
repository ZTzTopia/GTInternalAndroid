project(ModMenu LANGUAGES CXX)

option(KITTYMEMORY_DEBUG "Enable KittyMemory debug log" ON)

file(GLOB INCLUDE_FILES
    ../vendor/KittyMemory/*.h
    ../vendor/imgui/*.h
    ../vendor/imgui/backends/imgui_impl_opengl3.h
    game/*.h
    helper/*.h
    ui/*.h
    utils/*.h)

file(GLOB SOURCE_FILES
    ../vendor/imgui/*.cpp
    ../vendor/imgui/backends/imgui_impl_opengl3.cpp
    ../vendor/KittyMemory/*.c
    ../vendor/KittyMemory/*.cpp
    *.cpp
    game/*.cpp
    ui/*.cpp)

add_library(${PROJECT_NAME} SHARED
    ${INCLUDE_FILES}
    ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_FLAGS "-fomit-frame-pointer")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    IMGUI_IMPL_OPENGL_ES2)

target_include_directories(${PROJECT_NAME} PRIVATE
    ../vendor/imgui
    ../vendor/KittyMemory)

target_link_libraries(${PROJECT_NAME}
    android
    log
    GLESv2
    dobby)
