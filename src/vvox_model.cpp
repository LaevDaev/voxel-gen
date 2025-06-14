#include "vvox_model.hpp"

namespace vvox {
        VvoxModel::VvoxModel(VvoxDevice &device, const std::vector<Vertex> &vertices) : vvoxdevice{device} {
            createVertexBuffers(vertices);
        }
        VvoxModel::~VvoxModel() {
            vkDestroyBuffer(vvoxdevice.device(), vertexBuffer, nullptr);
            vkFreeMemory(vvoxdevice.device(), vertexBufferMemory, nullptr);
        }
        void VvoxModel::createVertexBuffers(const std::vector<Vertex> &vertices) {
            vertexCount = static_cast<uint32_t>(vertices.size());
            assert(vertexCount >=3 && "Vertex Count must be atleast 3");
            VkDeviceSize bufferSize = sizeof(vertices[0]) *vertexCount;

            vvoxdevice.createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                vertexBuffer,
                vertexBufferMemory
            );
            void *data;
            vkMapMemory(vvoxdevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
            vkUnmapMemory(vvoxdevice.device(), vertexBufferMemory);

        }

        
        void VvoxModel::bind(VkCommandBuffer commandBuffer) {
            VkBuffer buffers[] = {vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        }
        void VvoxModel::draw(VkCommandBuffer commandBuffer) {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
        std::vector<VkVertexInputBindingDescription> VvoxModel::Vertex::getBindingDescriptions() {
            std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
            bindingDescriptions[0].binding = 0;
            bindingDescriptions[0].stride = sizeof(Vertex);
            bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return  bindingDescriptions;
        }
        std::vector<VkVertexInputAttributeDescription> VvoxModel::Vertex::getAttributeDescriptions() {
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, position);
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);
            return attributeDescriptions;
        }


}