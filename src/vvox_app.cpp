#include "vvox_app.hpp"

namespace vvox
{
    void VvoxApp::run() {
        while(!vvoxWindow.shouldClose()) {
            glfwPollEvents();
        }
    }
} // namespace vvox