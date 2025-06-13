#include "vvox_app.hpp"

namespace vvox
{
    void VvoxApp::run() {
        while(!VvoxWindow.shouldClose()) {
            glfwPollEvents();
        }
    }
} // namespace vvox