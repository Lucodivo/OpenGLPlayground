cmake_minimum_required (VERSION 3.5)
project (LearnOpenGL)

set(LIB_DIR "C:/developer/dependencies/libs")

if(MSVC)
    add_compile_options(/W3)
else ()
    add_compile_options(-Wall -Wall -Wpedantic)
endif()

# Add .lib files
link_directories(${LIB_DIR})

# Add source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# STB_IMAGE
add_library(stb_image "${LIB_DIR}/stb_image.cpp")

# GLAD
add_library(glad "${LIB_DIR}/glad.c")

# Put all libraries into a variable
set(LIBS glfw3 opengl32 assimpd freetype stb_image glad)

# Define the include DIRs
include_directories(
        C:/developer/dependencies/include
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})