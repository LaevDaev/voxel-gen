#pragma once

#include "vvox_device.hpp"

namespace vvox
{
    class VvoxModel
    {
    public:
        VvoxModel();
        ~VvoxModel();

        VvoxModel(const VvoxModel &) = delete;
        VvoxModel &operator=(const VvoxModel &) = delete;

    private:
        VvoxDevice &vvoxdevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexCount;
    };
} // namespace vvox
