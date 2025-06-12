cmake_minimum_required(VERSION 3.15)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR Ninja)
# Vulkan SDK (ensure VULKAN_SDK env var is set)
find_package(Vulkan REQUIRED)

# Use FetchContent to get GLFW
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# Optional: GLM (math lib), stb (images)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# Add executable
add_executable(MyGame src/main.cpp)

# Include Vulkan, GLFW, GLM
target_link_libraries(MyGame PRIVATE Vulkan::Vulkan glfw glm)
target_include_directories(MyGame PRIVATE ${Vulkan_INCLUDE_DIR})

# For macOS Vulkan via MoltenVK
if(APPLE)
    find_library(MOLTENVK MoltenVK REQUIRED)
    target_link_libraries(MyGame PRIVATE ${MOLTENVK})
endif()
