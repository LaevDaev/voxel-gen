#pragma once

#include "vvox_device.hpp"

//std
#include <string>
#include <vector>

namespace vvox
{

    struct PipelineConfigInfo  {};
    class VvoxPipeline {
        public:
            VvoxPipeline(
                VvoxDevice &device,
                const std::string& vertFilepath,
                const std::string& fragFilepath,
                const PipelineConfigInfo& configInfo
            );
            ~VvoxPipeline() {}

            VvoxPipeline(const VvoxPipeline&) = delete;
            void operator=(const VvoxPipeline) = delete;

            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);


        private:
            static std::vector<char> readFile(const std::string& filepath);

            void createGraphicsPipeline(
                const std::string& vertFilepath,
                const std::string& fragFilepath,
                const PipelineConfigInfo& configInfo
            );

            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            VvoxDevice &vvoxDevice;
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
    };
} // namespace vvox
