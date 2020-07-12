cmake_minimum_required (VERSION 3.5)
project (LearnOpenGL)

set(LIB_DIR "C:/developer/dependencies/libs")
set(INCL_DIR "C:/developer/dependencies/include")

if(MSVC)
    add_compile_options(/W3)
else ()
    add_compile_options(-Wall -Wall -Wpedantic)
endif()

# Add .lib files
link_directories(${LIB_DIR})

# project source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)
# project header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Dear IMGUI source files
file(GLOB IMGUI_SOURCE_FILES
        ${INCL_DIR}/imgui/*.cpp)
# Dear IMGUI header files
file(GLOB IMGUI_HEADER_FILES
        ${INCL_DIR}/imgui/*.h)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${IMGUI_HEADER_FILES} ${IMGUI_SOURCE_FILES})

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
        ${INCL_DIR}
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})