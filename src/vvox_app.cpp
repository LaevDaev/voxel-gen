#include "vvox_app.hpp"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
//std
#include <stdexcept>
#include <array>
//#include <iostream>

namespace vvox
{
    struct SimplePushConstantData {
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    VvoxApp::VvoxApp()
    {
        loadGameObjects();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }
    VvoxApp::~VvoxApp()
    {
        vkDestroyPipelineLayout(vvoxDevice.device(), pipelineLayout, nullptr);
    }
    void VvoxApp::run()
    {
        //std::cout << "maxPushConstantSize = " << vvoxDevice.properties.limits.maxPushConstantsSize << "\n"; 
        while (!vvoxWindow.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(vvoxDevice.device());
    }
    void sierpinski(std::vector<VvoxModel::Vertex> *vertices, int depth, glm::vec2 bl, glm::vec2 t, glm::vec2 br)
    {
        if (depth < 1)
        {
            vertices->push_back({bl, {1.0f, 0.0f, 0.0f}});
            vertices->push_back({t, {0.0f, 1.0f, 0.0f}});
            vertices->push_back({br, {0.0f, 0.0f, 1.0f}});
            return;
        };
        glm::vec2 l, r, d;
        l = t + (bl - t) * 0.5f;
        r = t + (br - t) * 0.5f;
        d = br + (bl - br) * 0.5f;
        sierpinski(vertices, depth - 1, bl, l, d);
        sierpinski(vertices, depth - 1, l, t, r);
        sierpinski(vertices, depth - 1, d, r, br);
    }
    void myfrac(std::vector<VvoxModel::Vertex> *vertices, int depth, glm::vec2 left, glm::vec2 right)
    {
        if (depth < 1)
            return;
        vertices->push_back({left});
        auto v_temp = left - right;
        auto pp = right + (v_temp * 0.5f);
        auto t = -v_temp.y;
        v_temp.y = v_temp.x;
        v_temp.x = t;
        pp += v_temp * 0.2f;
        vertices->push_back({pp});
        vertices->push_back({right});
        myfrac(
            vertices,
            depth - 1,
            left,
            pp);
        myfrac(
            vertices,
            depth - 1,
            pp,
            right);
    }
    void VvoxApp::loadGameObjects()
    {
        std::vector<VvoxModel::Vertex> vertices;
        sierpinski(&vertices, 0, {-0.5f, 0.5f}, {0.0f, -0.5f}, {0.5f, 0.5f});
        auto vvoxModel = std::make_shared<VvoxModel>(vvoxDevice, vertices);
        
        for(int i = 0; i < 50 ; ++i) {
            

            auto triangle = VvoxGameObject::createGameObject();
            triangle.model = vvoxModel;
            triangle.color = {.1f + i * 0.01f, 0.1f + i * 0.01, 0.1f + i * 0.01f};
            //triangle.transform2d.translation.x = .0f + 0.01 * i * glm::sin(glm::two_pi<float>() * 10 / i );
            //triangle.transform2d.translation.y = .0f + 0.01 * i * glm::cos(glm::two_pi<float>() * 10 / i );

            triangle.transform2d.scale.x = .15f + 0.1f * i;
            triangle.transform2d.scale.y = .15f + 0.1f * i;
            triangle.transform2d.rotation = .25f * glm::two_pi<float>() * 0.5f * i;

        gameObjects.push_back(std::move(triangle));

        }
    }

    void VvoxApp::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(vvoxDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }
    void VvoxApp::createPipeline()
    {
        assert(vvoxSwapChain != nullptr && "cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "cannot create pipeline before pipelinelayout");

        PipelineConfigInfo pipelineConfig{};
        VvoxPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = vvoxSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        const std::string vertShaderPath = std::string(COMPILED_SHADERS_DIR) + "/simp_shader.vert.spv";
        const std::string fragShaderPath = std::string(COMPILED_SHADERS_DIR) + "/simp_shader.frag.spv";

        vvoxPipeline = std::make_unique<VvoxPipeline>(
            vvoxDevice,
            vertShaderPath,
            fragShaderPath,
            pipelineConfig);
    }
    void VvoxApp::recreateSwapChain() {
        auto extent = vvoxWindow.getExtend();
        while(extent.width == 0 || extent.height == 0) {
            extent = vvoxWindow.getExtend();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(vvoxDevice.device());
        if(vvoxSwapChain == nullptr) {
        vvoxSwapChain = std::make_unique<VvoxSwapChain>(vvoxDevice, extent);
    } else {
        vvoxSwapChain = std::make_unique<VvoxSwapChain>(vvoxDevice, extent, std::move(vvoxSwapChain));
        if(vvoxSwapChain->imageCount() != commandBuffers.size()) {
            freeCommandBuffers();
            createCommandBuffers();
        }    
    }
        //TODO CHECK IF RENDER PASS COMPATIBLE --> do nothing
        createPipeline();

    }
void VvoxApp::freeCommandBuffers() {
    vkFreeCommandBuffers(
        vvoxDevice.device(), 
        vvoxDevice.getCommandPool(), 
        static_cast<float>(commandBuffers.size()), 
        commandBuffers.data());
    commandBuffers.clear();
}

    void VvoxApp::createCommandBuffers()
    {
        commandBuffers.resize(vvoxSwapChain->imageCount());
        
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vvoxDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(vvoxDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers");
        }
    };

    void VvoxApp::recordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            std::runtime_error("failed to begin recording CommandBuffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vvoxSwapChain->getRenderPass();
        renderPassInfo.framebuffer = vvoxSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = vvoxSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(vvoxSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(vvoxSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0,0}, vvoxSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        renderGameObjects(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            std::runtime_error("failed to record to command Buffer");
        }
    }

    void VvoxApp::renderGameObjects(VkCommandBuffer commandBuffer) {
        vvoxPipeline->bind(commandBuffer);

        for ( auto& obj: gameObjects ) {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.1f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();
            vkCmdPushConstants(
                commandBuffer, 
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0, 
                sizeof(SimplePushConstantData),
                &push);
             obj.model->bind(commandBuffer);
             obj.model->draw(commandBuffer);
        }
    }

    void VvoxApp::drawFrame()
    {
        uint32_t imageIndex;
        auto result = vvoxSwapChain->acquireNextImage(&imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to aquire swap chain image!");
        }
        recordCommandBuffer(imageIndex);
        result = vvoxSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vvoxWindow.wasWindowResized()) {
            vvoxWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS)
        {
            std::runtime_error("failed to present swap chain image");
        }
    }
} // namespace vvox