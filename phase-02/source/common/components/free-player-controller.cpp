#include "free-player-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    // Reads sensitivities & speedupFactor from the given json object
    void FreePlayerControllerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        //rotationSensitivity = data.value("rotationSensitivity", rotationSensitivity);
        //fovSensitivity = data.value("fovSensitivity", fovSensitivity);
        positionSensitivity = data.value("positionSensitivity", positionSensitivity);
        //speedupFactor = data.value("speedupFactor", speedupFactor);
        boundary = data.value("boundary", boundary);
        is_bounded = data.value("is_bounded", is_bounded);
        total_count = data.value("total_count", total_count);
    }

}