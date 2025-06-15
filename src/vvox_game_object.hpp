#pragma once

#include "vvox_model.hpp"

#include <memory>

namespace vvox
{

    struct Transform2dComponent
    {
        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2()
        {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c, s}, {-s, c}};
            glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
            return rotMatrix * scaleMat;
        }
    };

    class VvoxGameObject
    {
    public:
        using id_t = unsigned int;
        static VvoxGameObject createGameObject()
        {
            static id_t currentId = 0;
            return VvoxGameObject{currentId++};
        }

        VvoxGameObject(const VvoxGameObject &) = delete;
        VvoxGameObject &operator=(const VvoxGameObject &) = delete;
        VvoxGameObject(VvoxGameObject &&) = default;
        VvoxGameObject &operator=(VvoxGameObject &&) = default;

        id_t getId() { return id; }

        std::shared_ptr<VvoxModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d;

    private:
        VvoxGameObject(id_t objId) : id{objId}
        {
        }
        id_t id;
    };
}