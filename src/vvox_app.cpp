#include "vvox_app.hpp"

#include <stdexcept>

namespace vvox
{
    VvoxApp::VvoxApp() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }
    VvoxApp::~VvoxApp() {
        vkDestroyPipelineLayout(vvoxDevice.device(), pipelineLayout, nullptr);
    }
    void VvoxApp::run()
    {
        while (!vvoxWindow.shouldClose())
        {
            glfwPollEvents();
        }
    }

    void VvoxApp::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(vvoxDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }
    void VvoxApp::createPipeline()
    {
        auto pipelineConfig = VvoxPipeline::defaultPipelineConfigInfo(vvoxSwapChain.width(), vvoxSwapChain.height());
        pipelineConfig.renderPass = vvoxSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        const std::string vertShaderPath = std::string(COMPILED_SHADERS_DIR) + "/simp_shader.vert.spv";
        const std::string fragShaderPath = std::string(COMPILED_SHADERS_DIR) + "/simp_shader.frag.spv";

        vvoxPipeline = std::make_unique<VvoxPipeline>(
            vvoxDevice, 
            vertShaderPath,
            fragShaderPath,
            pipelineConfig);
    }
    void VvoxApp::createCommandBuffers(){}; 
    void VvoxApp::drawFrame(){}; 
} // namespace vvox