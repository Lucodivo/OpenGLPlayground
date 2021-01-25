cmake_minimum_required (VERSION 3.5)
project (LearnOpenGL)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(LIB_DIR "C:/developer/dependencies/libs")
set(INCL_DIR "C:/developer/dependencies/include")
set(EXT_DIR "${CMAKE_SOURCE_DIR}/external" "${CMAKE_SOURCE_DIR}/external/glm" "${CMAKE_SOURCE_DIR}/external/imgui")

# Note: CMAKE_SOURCE_DIR is top level of the source tree (where top level CMakeLists.txt exists)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.h)

file(GLOB IMGUI_SOURCE_FILES ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp ${CMAKE_SOURCE_DIR}/external/imgui/*.h
                            ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.h ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
                            ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.h ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp )

link_directories(${LIB_DIR})
include_directories(${INCL_DIR} ${EXT_DIR})

add_library(glad STATIC "${LIB_DIR}/glad.c")

if(MSVC)
    add_compile_options(/W3)
else ()
    add_compile_options(-Wall -Wall -Wpedantic)
endif()

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${IMGUI_SOURCE_FILES})

# Put all libraries into a variable
set(LIBS glfw3-x64-d opengl32 glad assimp freetype)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})