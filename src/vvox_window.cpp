#include "vvox_window.hpp"
#include <iostream>
#include <stdexcept>

namespace vvox
{
    VvoxWindow::VvoxWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    VvoxWindow::~VvoxWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void VvoxWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }
    void VvoxWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto vvoxWindow = reinterpret_cast<VvoxWindow *>(glfwGetWindowUserPointer(window));
        vvoxWindow->framebufferResized = true;
        vvoxWindow->width = width;
        vvoxWindow->height = height;
    }
    void VvoxWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }

}
