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

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        bool shouldClose() { return glfwWindowShouldClose(window);}
    private:
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow *window;
    };
} //namespace end