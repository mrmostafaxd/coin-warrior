#pragma once
#include "../ecs/component.hpp"
#include <glm/glm.hpp> 

namespace our {
    class  FreePlayerControllerComponent : public Component {
    public:
        //float rotationSensitivity = 0.01f; // The angle change per pixel of mouse movement
        //float fovSensitivity = 0.3f; // The fov angle change per unit of mouse wheel scrolling
        glm::vec3 positionSensitivity = {3.0f, 3.0f, 3.0f}; // The unity per second of camera movement if WASD is pressed
        glm::vec2 boundary = { 0.0f, 0.0f};
        int is_bounded = 0;
        int coin_count = 0;
        int total_count = 0;
        bool obstacle_hit = false;
        //float speedupFactor = 5.0f; // A multiplier for the positionSensitivity if "Left Shift" is held.

        // The ID of this component type is "Free Player Controller"
        static std::string getID() { return "Free Player Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}