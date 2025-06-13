#pragma once


#include "vvox_pipeline.hpp"
#include "vvox_window.hpp"

namespace vvox
{
  class VvoxApp
  {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 800;

    void run();

  private:
    VvoxWindow VvoxWindow{WIDTH, HEIGHT, "Hello Vulkan!"};

    const std::string vertShaderPath = std::string(COMPILED_SHADERS_DIR) + "/simp_shader.vert.spv";
    const std::string fragShaderPath = std::string(COMPILED_SHADERS_DIR) + "/simp_shader.frag.spv";
    VvoxPipeline VvoxPipeline{vertShaderPath, fragShaderPath};
  };
} // namespace vvox
