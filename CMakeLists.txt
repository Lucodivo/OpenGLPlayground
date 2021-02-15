cmake_minimum_required (VERSION 3.5)
project (LearnOpenGL)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(LIB_DIR "C:/developer/dependencies/libs")
set(INCL_DIR "C:/developer/dependencies/include")
set(EXT_DIR "${CMAKE_SOURCE_DIR}/external")

# Note: CMAKE_SOURCE_DIR is top level of the source tree (where top level CMakeLists.txt exists)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.h)

file(GLOB IMGUI_SOURCE_FILES ${EXT_DIR}/imgui/*.cpp ${EXT_DIR}/imgui/*.h
                            ${EXT_DIR}/imgui/backends/imgui_impl_glfw.h ${EXT_DIR}/imgui/backends/imgui_impl_glfw.cpp
                            ${EXT_DIR}/imgui/backends/imgui_impl_opengl3.h ${EXT_DIR}/imgui/backends/imgui_impl_opengl3.cpp )

link_directories(${LIB_DIR})
include_directories(${INCL_DIR} ${EXT_DIR} "${EXT_DIR}/glm" "${EXT_DIR}/imgui")

add_library(glad STATIC "${EXT_DIR}/glad/glad.c" )

if(MSVC)
    add_compile_options(/W3)
else ()
    add_compile_options(-Wall -Wall -Wpedantic)
endif()

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${IMGUI_SOURCE_FILES})

# Put all libraries into a variable
set(LIBS glfw3-x64-d opengl32 glad assimp)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})