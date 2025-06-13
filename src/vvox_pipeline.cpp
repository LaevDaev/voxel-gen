#include "vvox_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
namespace vvox
{

    VvoxPipeline::VvoxPipeline(
        VvoxDevice &device,
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo) : vvoxDevice{device}
    {
        createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
    }

    std::vector<char> VvoxPipeline::readFile(const std::string &filepath)
    {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};
        if (!file.is_open())
        {
            throw std::runtime_error("failed to open shader file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }
    void VvoxPipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo& configInfo)
    {
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }
    void VvoxPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
        if(vkCreateShaderModule(vvoxDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module");
        }
    }
    PipelineConfigInfo VvoxPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
        PipelineConfigInfo configInfo{};
        return configInfo;
    }


} // namespace vvox
