#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace vvox
{
    class VvoxWindow
    {

    public:
        VvoxWindow(int w, int h, std::string name);
        ~VvoxWindow();

        VvoxWindow(const VvoxWindow &) = delete;
        VvoxWindow &operator=(const VvoxWindow &) = delete;
        bool wasWindowResized() { return framebufferResized; }
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
        void resetWindowResizedFlag() { framebufferResized = false; }
        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtend() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
        void initWindow();

        int width;
        int height;
        bool framebufferResized = false;

        std::string windowName;
        GLFWwindow *window;
    };
} // namespace end