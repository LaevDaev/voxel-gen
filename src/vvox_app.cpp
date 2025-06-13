#include "vvox_app.hpp"
#include <stdexcept>
#include <array>

namespace vvox
{
    VvoxApp::VvoxApp() {
        loadModels();
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
            drawFrame();
        }

        vkDeviceWaitIdle(vvoxDevice.device()); 
    }
    void myfrac(std::vector<VvoxModel::Vertex> *vertices, int depth, glm::vec2 left, glm::vec2 right) {
        if(depth < 1) return;
        vertices->push_back({left});
        auto v_temp = left - right;
        auto pp = right + (v_temp * 0.5f);
        auto t = - v_temp.y;
        v_temp.y = v_temp.x;
        v_temp.x = t;
        pp += v_temp * 0.2f;
        vertices->push_back({pp});
        vertices->push_back({right});
        myfrac(
            vertices,
            depth - 1,
            left,
            pp
        );
        myfrac(
            vertices,
            depth - 1,
            pp,
            right
        );

    }
    void VvoxApp::loadModels() {
        std::vector<VvoxModel::Vertex> vertices;
        myfrac(&vertices, 15, {-0.5f,0.5f}, {0.5f, 0.5f});
        vvoxmodel = std::make_unique<VvoxModel>(vvoxDevice, vertices);
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
    void VvoxApp::createCommandBuffers(){
        commandBuffer.resize(vvoxSwapChain.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};

        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        allocInfo.commandPool = vvoxDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffer.size());

        if(vkAllocateCommandBuffers(vvoxDevice.device(), &allocInfo, commandBuffer.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers");
        }

        for (int i = 0; i < commandBuffer.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            
            if(vkBeginCommandBuffer(commandBuffer[i], &beginInfo) != VK_SUCCESS) {
                std::runtime_error("failed to begin recording CommandBuffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = vvoxSwapChain.getRenderPass();
            renderPassInfo.framebuffer = vvoxSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = vvoxSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.01f,0.01f,0.01f,1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();


            vkCmdBeginRenderPass(commandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vvoxPipeline->bind(commandBuffer[i]);
            vvoxmodel->bind(commandBuffer[i]);
            vvoxmodel->draw(commandBuffer[i]);

            vkCmdEndRenderPass(commandBuffer[i]);
            if (vkEndCommandBuffer(commandBuffer[i]) != VK_SUCCESS) {
                std::runtime_error("failed to record to command Buffer");
            }

        }
    }; 
    void VvoxApp::drawFrame(){
        uint32_t imageIndex;
        auto result = vvoxSwapChain.acquireNextImage(&imageIndex);

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to aquire swap chain image!");
        }

        result = vvoxSwapChain.submitCommandBuffers(&commandBuffer[imageIndex], &imageIndex);
        if(result != VK_SUCCESS){
            std::runtime_error("failed to present swap chain image");
        }
    }
} // namespace vvox