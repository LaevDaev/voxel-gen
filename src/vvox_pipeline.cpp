#include "vvox_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
namespace vvox
{

    VvoxPipeline::VvoxPipeline(const std::string& vertFilepath, const std::string& fragFilepath)
    {
        createGraphicsPipeline(vertFilepath, fragFilepath);
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
    void VvoxPipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath)
    {
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }

} // namespace vvox
