cmake_minimum_required (VERSION 3.5)
project (LearnOpenGL)

if(MSVC)
    add_compile_options(/W3)
else ()
    add_compile_options(-Wall -Wall -Wpedantic)
endif()

# Add .lib files
link_directories(C:/developer/dependencies/OpenGL/libs)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/*.c
        ${CMAKE_SOURCE_DIR}/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/*.h
        ${CMAKE_SOURCE_DIR}/*.hpp)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# STB_IMAGE
add_library(STB_IMAGE "dependencies/stb_image.cpp")

# GLAD
add_library(GLAD "dependencies/glad.c")

# Put all libraries into a variable
set(LIBS glfw3 opengl32 assimpd freetype STB_IMAGE GLAD)

# Define the include DIRs
include_directories(
        "${CMAKE_SOURCE_DIR}/dependencies"
        C:/developer/dependencies/OpenGL/include
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})