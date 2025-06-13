#pragma once

#include "vvox_window.hpp"

namespace vvox
{
  class VvoxApp
  {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 800;

    void run();

  private:
    VvoxWindow VvoxWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  };
} // namespace vvox
