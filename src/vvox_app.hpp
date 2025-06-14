#pragma once

#include "vvox_pipeline.hpp"
#include "vvox_window.hpp"
#include "vvox_device.hpp"
#include "vvox_swap_chain.hpp"
#include "vvox_model.hpp"

#include <memory>
#include <vector>

namespace vvox
{
  class VvoxApp
  {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 800;
    VvoxApp();
    ~VvoxApp();

    VvoxApp(const VvoxApp &) = delete;
    VvoxApp &operator=(const VvoxApp &) = delete;

    void run();

  private:
    void loadModels();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int imageIndex);


    VvoxWindow vvoxWindow{WIDTH, HEIGHT, "Hello Vulkan!"};

    VvoxDevice vvoxDevice{vvoxWindow};
    std::unique_ptr<VvoxSwapChain> vvoxSwapChain;
    const std::string vertShaderPath = std::string(COMPILED_SHADERS_DIR) + "/simp_shader.vert.spv";
    const std::string fragShaderPath = std::string(COMPILED_SHADERS_DIR) + "/simp_shader.frag.spv";
    std::unique_ptr<VvoxPipeline> vvoxPipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<VvoxModel> vvoxmodel;
  };
} // namespace vvox
