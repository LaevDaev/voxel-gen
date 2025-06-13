cmake_minimum_required(VERSION 3.15)
project(Vvox)

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


file(GLOB SHADER_SOURCE_FILES
    "shaders/*.vert"
    "shaders/*.frag"
)

set(SPIRV_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/compiled_shaders/")
file(MAKE_DIRECTORY ${SPIRV_OUTPUT_DIR})

set(COMPILED_SHADERS "")

foreach(SHADER ${SHADER_SOURCE_FILES})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(SPIRV_FILE "${SPIRV_OUTPUT_DIR}/${FILE_NAME}.spv")

    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND glslc ${SHADER} -o ${SPIRV_FILE}
        DEPENDS ${SHADER}
        COMMENT "Compiling ${FILE_NAME} to SPIR-V"
        VERBATIM
    )

    list(APPEND COMPILED_SHADERS ${SPIRV_FILE})
endforeach()

file(RELATIVE_PATH COMPILED_SHADERS_REL_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/src"
     "${SPIRV_OUTPUT_DIR}"
)


add_custom_target(CompileShaders ALL DEPENDS ${COMPILED_SHADERS})

add_compile_definitions(COMPILED_SHADERS_DIR="${COMPILED_SHADERS_REL_PATH}")
# Add executable
add_executable(Vvox 
  src/main.cpp
  src/vvox_app.cpp
  src/vvox_window.cpp
  src/vvox_pipeline.cpp
  src/vvox_device.cpp
  src/vvox_swap_chain.cpp
  src/vvox_model.cpp
  )

add_dependencies(Vvox CompileShaders)

# Include Vulkan, GLFW, GLM
target_link_libraries(Vvox PRIVATE Vulkan::Vulkan glfw glm)
target_include_directories(Vvox PRIVATE ${Vulkan_INCLUDE_DIR})

# For macOS Vulkan via MoltenVK
if(APPLE)
    find_library(MOLTENVK MoltenVK REQUIRED)
    target_link_libraries(Vvox PRIVATE ${MOLTENVK})
endif()
