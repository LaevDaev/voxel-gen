#pragma once

#include "vvox_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include <cstring>

namespace vvox
{
    class VvoxModel
    {
    public:
        struct Vertex
        {
            glm::vec2 position;
            glm::vec3 color;
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        VvoxModel(VvoxDevice &device, const std::vector<Vertex> &vertices);
        ~VvoxModel();

        VvoxModel(const VvoxModel &) = delete;
        VvoxModel &operator=(const VvoxModel &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        VvoxDevice &vvoxdevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
} // namespace vvox
