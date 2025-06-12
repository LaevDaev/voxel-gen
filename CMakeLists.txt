cmake_minimum_required(VERSION 3.16)

# Default to Ninja if no generator is specified
if(NOT CMAKE_GENERATOR)
    set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
endif()

# Project name and C++ standard
project(MyCppProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Source files (edit as needed)
set(SOURCES
    src/main.cpp
    # Add more .cpp files here
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Find system-level Vulkan and GLFW
find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Include and link GLFW
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARIES})

# Link Vulkan
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

# Optional: additional warnings and debug info
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

# Print status
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "Output dir: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Using Vulkan: ${Vulkan_LIBRARY}")
message(STATUS "Using GLFW: ${GLFW_LIBRARIES}")
