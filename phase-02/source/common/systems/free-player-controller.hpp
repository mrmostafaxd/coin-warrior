
#include "../ecs/world.hpp"
#include "../components/free-player-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

namespace our {
    class  FreePlayerControllerSystem {
        Application* app; // The application in which the state runs

    public:
        // when a state enters, it should call this function and give it the pointer to the application 
        void enter(Application* app){
            this->app = app;
        }

        // this should be called every frame to update all entities containing a FreePlayerControllerComponent
        void update(World* world, float deltaTime) {
            // first of all, we search for an entity containing a FreePlayerControllerComponent`
            // as soon as we find one, we break
            FreePlayerControllerComponent *controller = nullptr;
            for(auto entity : world->getEntities()){
                controller = entity->getComponent<FreePlayerControllerComponent>();
                if(controller) break;
            }

            // if there is no entity with a FreePlayerControllerComponent, we can do nothing so we return
            if(!controller) return;

            //std::cout << controller->coin_count << std::endl;

            // check win state and lose staet
            if(controller->coin_count >= controller->total_count){
                app->changeState("win");
            }
            if(controller->obstacle_hit){
                app->changeState("lose");
            }


            // get the entity that we found via getOwner of controller
            Entity* entity = controller->getOwner();

            // we get a reference to the entity's position
            glm::vec3& position = entity->localTransform.position;

            // We get the player model matrix (relative to its parent) to compute the front and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));
            
            glm::vec3 current_sensitivity = controller->positionSensitivity;
            glm::vec2 current_boundary = controller->boundary;
            int is_bounded = controller->is_bounded;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if(app->getKeyboard().isPressed(GLFW_KEY_W)) position += front * (deltaTime * current_sensitivity.z);
            if(app->getKeyboard().isPressed(GLFW_KEY_S)) position -= front * (deltaTime * current_sensitivity.z);
            // A & D moves the player left or right (setting boundaries)
            if(app->getKeyboard().isPressed(GLFW_KEY_D)) {
                std::cout << "The current position is: " << position.x << std::endl;
                if(is_bounded == 1) {
                    if(position.x < current_boundary.y) {
                        position += right * (deltaTime * current_sensitivity.x);
                    }
                } else {
                    position += right * (deltaTime * current_sensitivity.x);
                }  
                //position += right * (deltaTime * current_sensitivity.x);
            } 
            if(app->getKeyboard().isPressed(GLFW_KEY_A))
            {
                std::cout << "The current position is: " << position.x << std::endl;
                if(is_bounded == 1) {
                    // std::cout << "The current position is: " << position.x << std::endl;
                    // std::cout << "The current boundary is: " << current_boundary.x << " " << current_boundary.y << std::endl;
                    if(position.x >  current_boundary.x) {
                        position -= right * (deltaTime * current_sensitivity.x);
                    }
                } else {
                    position -= right * (deltaTime * current_sensitivity.x);
                }  
                //position -= right * (deltaTime * current_sensitivity.x);
            } 
        }

    };
}